#!/usr/bin/env python3
# Python file to run all experiments in a paper published at the TAP 2024 conference.
#
# Note that this script is pretty much tailor-made for the example in the paper, in that the parser for the output of SPOT
# only covers the cases needed for this case study. Also, the right order of the atomic propositions for the monitor
# is hard-coded in this script below.
#
# This script is dual-licensed under licensed under CC BY-SA version 4.0, available at 
# https://creativecommons.org/licenses/by-sa/4.0/legalcode and the following BSD licence:
#
# ------------------------------------------[BSD License Text]---------------------------------------------
# Copyright 2024 Ruediger Ehlers
# 
# Redistribution and use in source and binary forms, with or without modification, are permitted provided 
# that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions and 
# the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
# the following disclaimer in the documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or 
# promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED 
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN 
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os, sys

# =========================================
# Settings for the scenario
# =========================================
APs = ["r1","y1","g1","r2","y2","g2","a1","a2"]

# Open output file
outFile = open("resultsTexMacros.tex","w")
USE_NUCLEO_BOARD = True # Run experiments on the board?

if USE_NUCLEO_BOARD:
    import serial, glob

# Reading from input file
specBlocks = []
with open("spec/starting_spec.txt","r") as inFile:
    allLines = inFile.readlines()
    currentBlock = []
    for line in allLines:
        line = line.strip()
        if len(line)==0:
            if len(currentBlock)>0:
                specBlocks.append(currentBlock)
                currentBlock = []
        else:
            currentBlock.append(line)
    if len(currentBlock)>0:
        specBlocks.append(currentBlock)

# =====================================
# Misc. Helper functions
# =====================================
def getNofStatesFromSpotFile(filename):
    with open(filename,"r") as inFile:
        for line in inFile.readlines():
            if line.startswith("States: "):
                nofStates = line.split(" ")[1].strip()
                return nofStates
    raise Exception("Error: Didn't find number of states in file: "+filename)

def getSizesForF446RE(inFile):
    foundSuccess = False
    for line in open(inFile).readlines():
        line = line.strip()
        if line.find("[SUCCESS]")!=-1:
            foundSuccess=True
        if line.startswith("RAM:"):
            dataRAM = int(line[line.find("(used"):].split(" ")[1])
        if line.startswith("Flash:"):
            dataFlash = int(line[line.find("(used"):].split(" ")[1])
    assert foundSuccess
    return (dataRAM,dataFlash)

def getFlashOverflowSizeForF446RE(inFile):
    foundOverflow = False
    for line in open(inFile).readlines():
        line = line.strip()
        if line.find("section `.text' will not fit in region `FLASH'")!=-1:
            foundOverflow=True
        if line.find("region `FLASH' overflowed by")!=-1:
            dataFlash = int(line.strip().split(" ")[-2])
    assert foundOverflow
    return dataFlash + 512*1024 # Add to FLASH memory of F446RE

def getNofLinesFromFile(inFile):
    nofLines = 0
    for line in open(inFile).readlines():
        nofLines += 1
    return nofLines

# =====================================
# Simple monitor compilers
# =====================================
def monitorCompilerNondeterministic(filenameIn,filenameOut):
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        with open(filenameIn,"r") as inFile:
            allInLines = [a.strip() for a in inFile.readlines()]
        
        # Gather header information
        apMapper = {}
        statesToTransitionsMapper = {}
        currentTransitionList = None

        for line in allInLines:
            if line.startswith("States: "):
                nofStates = int(line.split(" ")[1].strip())
            elif line.startswith("AP: "):
                parts = line.split(" ")
                for i,a in enumerate(parts[2:]):
                    for j,b in enumerate(APs):
                        if a=="\""+b+"\"":
                            apMapper[i] = j
                    if not i in apMapper:
                        raise Exception("Error: AP list in the script seems to be incorrect.")
            elif line.startswith("Start:"):
                startingStates = set([int(a) for a in line[7:].split(" ")])
            elif line.startswith("State:"):
                stateNum = int(line.split(" ")[1])
                assert not stateNum in statesToTransitionsMapper
                currentTransitionList = []
                statesToTransitionsMapper[stateNum] = currentTransitionList
            elif line.startswith("["):
                condition = line[0:line.find("]")+1]
                destinationState = int(line[line.find("]")+1:].strip().split(" ")[0])
                currentTransitionList.append((destinationState,condition))

        # Build state information
        outFile.write("/* State storage information */\n")
        for i in range(0,nofStates):
            if i in startingStates:
                outFile.write("uint8_t inState"+str(i)+" = 1;\n")
            else:
                outFile.write("uint8_t inState"+str(i)+" = 0;\n")

        # Build transition function
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for i in range(0,nofStates):
            outFile.write("  uint8_t nextState"+str(i)+" = 0;\n")
        for state in range(nofStates):
            outFile.write("  if (inState"+str(state)+"){\n")
            for (target,condition) in statesToTransitionsMapper[state]:
                assert condition[0]=="["
                assert condition[-1]=="]"
                condition = condition[1:-1]
                disjuncts = condition.split("|")
                outFile.write("    if (")
                for num,disjunct in enumerate(disjuncts):
                    if num>0:
                        outFile.write("||")
                    if len(disjuncts)!=1:
                        outFile.write("(")
                    for num2,conjunct in enumerate(disjunct.split("&")):
                        if num2>0:
                            outFile.write("&&")
                        conjunct = conjunct.strip()
                        if conjunct[0]=="!":
                            outFile.write("!")
                            conjunct = conjunct[1:]
                        apNum = int(conjunct)
                        outFile.write(APs[apMapper[apNum]])
                    if len(disjuncts)!=1:
                        outFile.write(")")
                outFile.write(") nextState"+str(target)+" = 1;\n")
            outFile.write("  }\n")
        for i in range(0,nofStates):
            outFile.write("  inState"+str(i)+" = nextState"+str(i)+";\n")
        for i in range(0,nofStates):
            outFile.write("  if (inState"+str(i)+") return 0;\n")
        outFile.write("  return 1; /* Reporting a violation. */\n")
        outFile.write("}\n")


def monitorCompilerUniversal(filenameIn,filenameOut):
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        with open(filenameIn,"r") as inFile:
            allInLines = [a.strip() for a in inFile.readlines()]
        
        # Gather header information
        apMapper = {}
        statesToTransitionsMapper = {}
        currentTransitionList = None

        for line in allInLines:
            if line.startswith("States: "):
                nofStates = int(line.split(" ")[1].strip())
            elif line.startswith("AP: "):
                parts = line.split(" ")
                for i,a in enumerate(parts[2:]):
                    for j,b in enumerate(APs):
                        if a=="\""+b+"\"":
                            apMapper[i] = j
                    if not i in apMapper:
                        raise Exception("Error: AP list in the script seems to be incorrect.")
            elif line.startswith("Start:"):
                startingStates = set([int(a) for a in line[7:].split(" ")])
            elif line.startswith("State:"):
                stateNum = int(line.split(" ")[1])
                assert not stateNum in statesToTransitionsMapper
                currentTransitionList = []
                statesToTransitionsMapper[stateNum] = currentTransitionList
            elif line.startswith("["):
                condition = line[0:line.find("]")+1]
                destinationState = int(line[line.find("]")+1:].strip().split(" ")[0])
                currentTransitionList.append((destinationState,condition))

        # Find rejecting state
        rejectingState = None
        for state in range(nofStates):
            if len(statesToTransitionsMapper[state])==1:
                if statesToTransitionsMapper[state][0][0]==state and statesToTransitionsMapper[state][0][1]=="[t]":
                    assert rejectingState is None
                    rejectingState = state
        assert not rejectingState is None


        # Build state information
        outFile.write("/* State storage information */\n")
        for i in range(0,nofStates):
            if i in startingStates:
                outFile.write("uint8_t inState"+str(i)+" = 1;\n")
            else:
                outFile.write("uint8_t inState"+str(i)+" = 0;\n")

        # Build transition function
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for i in range(0,nofStates):
            outFile.write("  uint8_t nextState"+str(i)+" = 0;\n")
        for state in range(nofStates):
            outFile.write("  if (inState"+str(state)+"){\n")
            for (target,condition) in statesToTransitionsMapper[state]:
                assert condition[0]=="["
                assert condition[-1]=="]"
                condition = condition[1:-1]
                disjuncts = condition.split("|")
                outFile.write("    if (")
                for num,disjunct in enumerate(disjuncts):
                    if num>0:
                        outFile.write("||")
                    if len(disjuncts)!=1:
                        outFile.write("(")
                    for num2,conjunct in enumerate(disjunct.split("&")):
                        if num2>0:
                            outFile.write("&&")
                        conjunct = conjunct.strip()
                        if conjunct=="t":
                            outFile.write("1")
                        else:
                            if conjunct[0]=="!":
                                outFile.write("!")
                                conjunct = conjunct[1:]
                            apNum = int(conjunct)
                            outFile.write(APs[apMapper[apNum]])
                    if len(disjuncts)!=1:
                        outFile.write(")")
                outFile.write(") nextState"+str(target)+" = 1;\n")
            outFile.write("  }\n")
        for i in range(0,nofStates):
            outFile.write("  inState"+str(i)+" = nextState"+str(i)+";\n")
        outFile.write("  return inState"+str(rejectingState)+";\n")
        outFile.write("}\n")


def monitorCompilerDeterministic(filenameIn,filenameOut):
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        with open(filenameIn,"r") as inFile:
            allInLines = [a.strip() for a in inFile.readlines()]
        
        # Gather header information
        apMapper = {}
        statesToTransitionsMapper = {}
        currentTransitionList = None

        for line in allInLines:
            if line.startswith("States: "):
                nofStates = int(line.split(" ")[1].strip())
            elif line.startswith("AP: "):
                parts = line.split(" ")
                for i,a in enumerate(parts[2:]):
                    for j,b in enumerate(APs):
                        if a=="\""+b+"\"":
                            apMapper[i] = j
                    if not i in apMapper:
                        raise Exception("Error: AP list in the script seems to be incorrect.")
            elif line.startswith("Start:"):
                startingStates = set([int(a) for a in line[7:].split(" ")])
            elif line.startswith("State:"):
                stateNum = int(line.split(" ")[1])
                assert not stateNum in statesToTransitionsMapper
                currentTransitionList = []
                statesToTransitionsMapper[stateNum] = currentTransitionList
            elif line.startswith("["):
                condition = line[0:line.find("]")+1]
                destinationState = int(line[line.find("]")+1:].strip().split(" ")[0])
                currentTransitionList.append((destinationState,condition))

        # Build state information
        outFile.write("/* State storage information */\n")
        assert len(startingStates)==1
        for a in startingStates:
            outFile.write("uint32_t monitorState = "+str(a)+";\n")

        # Build transition function
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for state in range(nofStates):
            outFile.write("  if (monitorState=="+str(state)+") {\n")
            for (target,condition) in statesToTransitionsMapper[state]:
                assert condition[0]=="["
                assert condition[-1]=="]"
                condition = condition[1:-1]
                disjuncts = condition.split("|")
                outFile.write("    if (")
                for num,disjunct in enumerate(disjuncts):
                    if num>0:
                        outFile.write("||")
                    if len(disjuncts)!=1:
                        outFile.write("(")
                    for num2,conjunct in enumerate(disjunct.split("&")):
                        if num2>0:
                            outFile.write("&&")
                        conjunct = conjunct.strip()
                        if conjunct[0]=="!":
                            outFile.write("!")
                            conjunct = conjunct[1:]
                        apNum = int(conjunct)
                        outFile.write(APs[apMapper[apNum]])
                    if len(disjuncts)!=1:
                        outFile.write(")")
                outFile.write(") { monitorState="+str(target)+"; return 0;")
                outFile.write("}\n")
            outFile.write("  }\n")
        outFile.write("  return 1; /* Fall through. */\n")
        outFile.write("}\n")


def monitorCompilerFragmented(filenamesIn,filenameOut):
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        
        apMapperFragments = [{} for a in filenamesIn]
        statesToTransitionsMapperFragments = [{} for a in filenamesIn]
        nofStates = {}
        startingStates = {}
        
        for fragment,filenameIn in enumerate(filenamesIn):
            with open(filenameIn,"r") as inFile:
                allInLines = [a.strip() for a in inFile.readlines()]
            
            # Gather header information
            currentTransitionList = None
            for line in allInLines:
                if line.startswith("States: "):
                    nofStates[fragment] = int(line.split(" ")[1].strip())
                elif line.startswith("AP: "):
                    parts = line.split(" ")
                    for i,a in enumerate(parts[2:]):
                        for j,b in enumerate(APs):
                            if a=="\""+b+"\"":
                                apMapperFragments[fragment][i] = j
                        if not i in apMapperFragments[fragment]:
                            raise Exception("Error: AP list in the script seems to be incorrect.")
                elif line.startswith("Start:"):
                    startingStates[fragment] = set([int(a) for a in line[7:].split(" ")])
                elif line.startswith("State:"):
                    stateNum = int(line.split(" ")[1])
                    assert not stateNum in statesToTransitionsMapperFragments[fragment]
                    currentTransitionList = []
                    statesToTransitionsMapperFragments[fragment][stateNum] = currentTransitionList
                elif line.startswith("["):
                    condition = line[0:line.find("]")+1]
                    destinationState = int(line[line.find("]")+1:].strip().split(" ")[0])
                    currentTransitionList.append((destinationState,condition))

        # Build state information
        outFile.write("/* State storage information */\n")
        for fragment in range(len(filenamesIn)):
            for i in range(0,nofStates[fragment]):
                if i in startingStates[fragment]:
                    outFile.write("uint8_t inState"+str(fragment)+"_"+str(i)+" = 1;\n")
                else:
                    outFile.write("uint8_t inState"+str(fragment)+"_"+str(i)+" = 0;\n")

        # Build transition function
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for fragment in range(len(filenamesIn)):
            for i in range(0,nofStates[fragment]):
                outFile.write("  uint8_t nextState"+str(fragment)+"_"+str(i)+" = 0;\n")
            for state in range(nofStates[fragment]):
                outFile.write("  if (inState"+str(fragment)+"_"+str(state)+"){\n")
                for (target,condition) in statesToTransitionsMapperFragments[fragment][state]:
                    assert condition[0]=="["
                    assert condition[-1]=="]"
                    condition = condition[1:-1]
                    disjuncts = condition.split("|")
                    outFile.write("    if (")
                    for num,disjunct in enumerate(disjuncts):
                        if num>0:
                            outFile.write("||")
                        if len(disjuncts)!=1:
                            outFile.write("(")
                        for num2,conjunct in enumerate(disjunct.split("&")):
                            if num2>0:
                                outFile.write("&&")
                            conjunct = conjunct.strip()
                            if conjunct[0]=="!":
                                outFile.write("!")
                                conjunct = conjunct[1:]
                            apNum = int(conjunct)
                            outFile.write(APs[apMapperFragments[fragment][apNum]])
                        if len(disjuncts)!=1:
                            outFile.write(")")
                    outFile.write(") nextState"+str(fragment)+"_"+str(target)+" = 1;\n")
                outFile.write("  }\n")
            for i in range(0,nofStates[fragment]):
                outFile.write("  inState"+str(fragment)+"_"+str(i)+" = nextState"+str(fragment)+"_"+str(i)+";\n")
            outFile.write("if (")
            for i in range(0,nofStates[fragment]):
                if i>0:
                    outFile.write(" && ")
                outFile.write("(!(inState"+str(fragment)+"_"+str(i)+"))")
            outFile.write(") return 1;\n")

        outFile.write("  return 0;\n")
        outFile.write("}\n")


# Get PlatformIO size base
with open("pioproject/src/monitor.c","w") as monitorCFile:
    monitorCFile.write("#include <stdint.h>\nint monitor("+",".join(["uint8_t "+a for a in APs])+") {}\n")
assert os.system("cd pioproject; pio run > /tmp/pioout 2>&1")==0
(baseRAM,baseFlash) = getSizesForF446RE("/tmp/pioout")
print("Base sizes RAM/Flash: ",baseRAM,baseFlash)
if USE_NUCLEO_BOARD:
    assert os.system("cd pioproject; pio run --target upload > /tmp/pioout2 2>&1")==0
    port = serial.Serial(glob.glob("/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink*")[0],baudrate=115200)
    nofCyclesEmpty = port.readline().strip().decode ("utf-8")
    port.close()
    assert nofCyclesEmpty.startswith("#Cycles:")
    nofCyclesEmpty = int(nofCyclesEmpty.split(" ")[1])
    print("Nof cycles empty execution: ",nofCyclesEmpty)
    
# Preparation for c<ses where all specification blocks are merged
specLinesOfAllBlocks = []
for a in specBlocks:
    specLinesOfAllBlocks.extend(a)
specOfAllBlocks = " & ".join(["("+a+")" for a in specLinesOfAllBlocks])

# Experiment 1: SPOT Compilation to a single NBA
assert os.system("lib/spot-2.12/bin/ltl2tgba -f \""+specOfAllBlocks+"\" > results/monolithic_nba.txt")==0
outFile.write("\\newcommand{\\nofStatesMonolithic}{"+getNofStatesFromSpotFile("results/monolithic_nba.txt")+"}\n")
monitorCompilerNondeterministic("results/monolithic_nba.txt","results/monolithic_nba.c")
outFile.write("\\newcommand{\\nofLinesMonolithic}{"+str(getNofLinesFromFile("results/monolithic_nba.c"))+"}\n")
# We run out of memory both with optimization turned on and off in the next 4 lines.
# assert os.system("cp results/monolithic_nba.c pioproject/src/monitor.c")==0
# assert os.system("cd pioproject; pio run -e nucleo_f446re_noopt > /tmp/pioout 2>&1")!=0
# newFlash = getFlashOverflowSizeForF446RE("/tmp/pioout")
# outFile.write("\\newcommand{\\FlashNonDeterministicDoesNotFit}{"+str(newFlash-baseFlash)+"}\n")


# Experiment 2: SPOT Compilation to a single det. Monitor
assert os.system("lib/spot-2.12/bin/ltl2tgba -M -f \""+specOfAllBlocks+"\" > results/monolithic_det_monitor.txt")==0
outFile.write("\\newcommand{\\nofStatesDeterministic}{"+getNofStatesFromSpotFile("results/monolithic_det_monitor.txt")+"}\n")
monitorCompilerDeterministic("results/monolithic_det_monitor.txt","results/monolithic_det_monitor.c")
# We need to run the following without GCC optimization, as otherwise
# GCC runs out of memory.
assert os.system("cp results/monolithic_det_monitor.c pioproject/src/monitor.c")==0
assert os.system("cd pioproject; pio run -e nucleo_f446re_noopt > /tmp/pioout 2>&1")!=0
newFlash = getFlashOverflowSizeForF446RE("/tmp/pioout")
outFile.write("\\newcommand{\\FlashDeterministicDoesNotFit}{"+str(newFlash-baseFlash)+"}\n")

# Experiment 3: Compilation of multiple monitor blocks
totalNumStatesBlocks = 0
for blockNum,block in enumerate(specBlocks):
    assert os.system("lib/spot-2.12/bin/ltl2tgba -M -f \""+" & ".join(["("+a+")" for a in block])+"\" > results/split_monitor"+str(blockNum)+".txt")==0
    nofAdditionalStates = getNofStatesFromSpotFile("results/split_monitor"+str(blockNum)+".txt")
    totalNumStatesBlocks += int(nofAdditionalStates)
outFile.write("\\newcommand{\\totalNofStatesFragmented}{"+str(totalNumStatesBlocks)+"}\n")
monitorCompilerFragmented(["results/split_monitor"+str(blockNum)+".txt" for blockNum in range(len(specBlocks))],"results/fragmented_monitor.c")
assert os.system("cp results/fragmented_monitor.c pioproject/src/monitor.c")==0
assert os.system("cd pioproject; pio run > /tmp/pioout 2>&1")==0
(newRAM,newFlash) = getSizesForF446RE("/tmp/pioout")
outFile.write("\\newcommand{\\RAMFragmented}{"+str(newRAM-baseRAM)+"}\n")
outFile.write("\\newcommand{\\FlashFragmented}{"+str(newFlash-baseFlash)+"}\n")
if USE_NUCLEO_BOARD:
    assert os.system("cd pioproject; pio run --target upload > /tmp/pioout2 2>&1")==0
    port = serial.Serial(glob.glob("/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink*")[0],baudrate=115200)
    nofCyclesThis = port.readline().strip().decode ("utf-8")
    port.close()
    assert nofCyclesThis.startswith("#Cycles:")
    nofCyclesThis = int(nofCyclesThis.split(" ")[1])
    outFile.write("\\newcommand{\\CyclesFragmented}{"+str((nofCyclesThis-nofCyclesEmpty)//7)+"}\n")
    
# Experiment 4: Universal automaton
assert os.system("lib/spot-2.12/bin/ltl2tgba -f \"!("+specOfAllBlocks+")\" > results/uca.txt")==0
assert os.system("lib/spot-2.12/bin/ltl2tgba -f \"!("+specOfAllBlocks+")\" -d > results/uca.dot")==0
outFile.write("\\newcommand{\\nofStatesUCW}{"+getNofStatesFromSpotFile("results/uca.txt")+"}\n")    
monitorCompilerUniversal("results/uca.txt","results/uca.c")
assert os.system("cp results/uca.c pioproject/src/monitor.c")==0
assert os.system("cd pioproject; pio run > /tmp/pioout 2>&1")==0
(newRAM,newFlash) = getSizesForF446RE("/tmp/pioout")
outFile.write("\\newcommand{\\RAMUniversal}{"+str(newRAM-baseRAM)+"}\n")
outFile.write("\\newcommand{\\FlashUniversal}{"+str(newFlash-baseFlash)+"}\n")
if USE_NUCLEO_BOARD:
    assert os.system("cd pioproject; pio run --target upload > /tmp/pioout2 2>&1")==0
    port = serial.Serial(glob.glob("/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink*")[0],baudrate=115200)
    nofCyclesThis = port.readline().strip().decode ("utf-8")
    port.close()
    assert nofCyclesThis.startswith("#Cycles:")
    nofCyclesThis = int(nofCyclesThis.split(" ")[1])
    outFile.write("\\newcommand{\\CyclesUniversal}{"+str((nofCyclesThis-nofCyclesEmpty)//7)+"}\n")

# Finally, statistics on this script
outFile.write("\\newcommand{\\LinesBuildMonitorsScript}{"+str(len([a for a in open("build_monitors.py","r").readlines() if len(a.strip())>0 and not a.startswith("#")]))+"}\n")
