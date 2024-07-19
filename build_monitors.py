#!/usr/bin/env python3
# Python file to run all experiments in a paper published at the TAP 2024 conference.
import os, sys

# =========================================
# Settings for the scenario
# =========================================
APs = ["r1","y1","g1","r2","y2","g2","a1","a2"]


# Open output file
outFile = open("resultsTexMacros.tex","w")

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
        outFile.write("\n/* Monitor step function */\n")
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
        outFile.write("  return 1; /* Violation if hot having returned earlier. */\n")
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
        outFile.write("\n/* Monitor step function */\n")
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
        outFile.write("\n/* Monitor step function */\n")
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


if False:
    # Experiment 1: SPOT Compilation to a single NBA
    specLinesOfAllBlocks = []
    for a in specBlocks:
        specLinesOfAllBlocks.extend(a)
    specOfAllBlocks = " & ".join(["("+a+")" for a in specLinesOfAllBlocks])
    assert os.system("lib/spot-2.12/bin/ltl2tgba -f \""+specOfAllBlocks+"\" > results/monolithic_nba.txt")==0
    outFile.write("\\newcommand{\\nofStatesMonolithic}{"+getNofStatesFromSpotFile("results/monolithic_nba.txt")+"}")
    monitorCompilerNondeterministic("results/monolithic_nba.txt","results/monolithic_nba.c")

    # Experiment 2: SPOT Compilation to a single det. Monitor
    assert os.system("lib/spot-2.12/bin/ltl2tgba -M -f \""+specOfAllBlocks+"\" > results/monolithic_det_monitor.txt")==0
    outFile.write("\\newcommand{\\nofStatesDeterministic}{"+getNofStatesFromSpotFile("results/monolithic_det_monitor.txt")+"}")
    monitorCompilerDeterministic("results/monolithic_det_monitor.txt","results/monolithic_det_monitor.c")

# Experiment 3: Compilation of multiple monitor blocks
totalNumStatesBlocks = 0
for blockNum,block in enumerate(specBlocks):
    assert os.system("lib/spot-2.12/bin/ltl2tgba -M -f \""+" & ".join(["("+a+")" for a in block])+"\" > results/split_monitor"+str(blockNum)+".txt")==0
    nofAdditionalStates = getNofStatesFromSpotFile("results/split_monitor"+str(blockNum)+".txt")
    totalNumStatesBlocks += int(nofAdditionalStates)
outFile.write("\\newcommand{\\totalNofStatesFragmented}{"+str(totalNumStatesBlocks)+"}")
monitorCompilerFragmented(["results/split_monitor"+str(blockNum)+".txt" for blockNum in range(len(specBlocks))],"results/fragmented_monitor.c")

