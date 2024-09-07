#!/usr/bin/env python3
# Python file to run one experiments in a paper published at the TAP 2024 conference.
#
# Used for showing an example of tracking violation information.
# This script *only* works if the automaton for the specifiation is very-weak.
# Otherwise the script will fail (with an error message, though).
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

# User settings -- full analysis?
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


def monitorCompilerUniversalWithReasonTracking(filenameIn,filenameOut):
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        outFile.write("#include <stdio.h>\n")
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

        # Tracking states works by multiplying the incoming state information by #incoming
        # edges, and then adding the incoming edge number
        # Also store max #transitions leading here
        # Note th
        maxLengthIncomingTracePerState = {a:0 for a in startingStates}
        todo = [a for a in startingStates]
        while len(todo)>0:
            thisOne = todo[0]
            todo = todo[1:]
            thisValue = maxLengthIncomingTracePerState[thisOne]
            for (a,b) in statesToTransitionsMapper[thisOne]:
                if not a in maxLengthIncomingTracePerState:
                    maxLengthIncomingTracePerState[a] = thisValue+1
                    todo.append(a)
                else:
                    if not a==thisOne:
                        if maxLengthIncomingTracePerState[thisOne]<thisValue+1:
                            maxLengthIncomingTracePerState[a] = thisValue+1
                            if not a in todo:
                                todo.append(a)
                            if thisValue>100:
                                raise Exception("Error: The input automaton is not very weak")
        

        # Find rejecting state
        rejectingState = None
        for state in range(nofStates):
            if len(statesToTransitionsMapper[state])==1:
                if statesToTransitionsMapper[state][0][0]==state and statesToTransitionsMapper[state][0][1]=="[t]":
                    assert rejectingState is None
                    rejectingState = state
        assert not rejectingState is None

        # Data type for AP values:
        if len(APs)<=8:
            dataTypeAPValues = "uint8_t"
        else:
            dataTypeAPValues = "uint32_t"
            assert len(APs)<=32

        # Build state information
        outFile.write("/* State storage information */\n")
        for i in range(0,nofStates):
            if i in startingStates:
                outFile.write("uint8_t inState"+str(i)+" = 1;\n")
            else:
                outFile.write("uint8_t inState"+str(i)+" = 0;\n")
            outFile.write("volatile uint32_t state"+str(i)+"history = 0;\n")
            # Backup.
            outFile.write("typedef struct {\n")
            for j in range(maxLengthIncomingTracePerState[i]):
                outFile.write("  "+dataTypeAPValues+"  vals"+str(j)+";\n")
            outFile.write("} state"+str(i)+"apValuesType;\n")
            outFile.write("volatile state"+str(i)+"apValuesType state"+str(i)+"apValues;\n")

        # Construct matro to compress APs to integer data type used for trace
        # storage
        outFile.write("#define CONSTRUCTTRACEELEMENT (")
        for i,a in enumerate(APs):
            if i>0:
                outFile.write("+")
            outFile.write("("+a+">0?"+str(1 << i)+":0)")
        outFile.write(")\n")

        # Build a revserse transition relation, so that we can 
        # access for each state which incoming states there are
        reverseTransitionRelation = [[] for a in statesToTransitionsMapper]
        for (a,b) in statesToTransitionsMapper.items():
            for c in b:
                reverseTransitionRelation[c[0]].append((a,c[1]))

        # Check assumption that there is no incoming edge in an initial state that
        # is not a self-loop -- the debug trace printer below doesn't work otherwise.
        for a in startingStates:
            for (b,c) in reverseTransitionRelation[a]:
                assert b==a

        # Build a topological sort of the depth in the weak automaton, so that
        # we can go leaf-to-root in the construction
        reverseTopologyPre = {}
        for (a,b) in maxLengthIncomingTracePerState.items():
            if not b in reverseTopologyPre:
                reverseTopologyPre[b] = [a]
            else:
                reverseTopologyPre[b].append(a)
        allLengths = list(set([a for (a,b) in reverseTopologyPre.items()]))
        allLengths.sort(reverse=True)
        topologicalStateSort = []
        for a in allLengths:
            topologicalStateSort.extend(reverseTopologyPre[a])
        
        # Build transition function
        # Use topological sorting, so that "nextState" variables are not needed.
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for state in topologicalStateSort:
            # First: process self-loops
            foundSelfLoop = False
            for (target,condition) in statesToTransitionsMapper[state]:
                if (target==state):
                    if foundSelfLoop:
                        raise Exception("Error: Multiple self-loops are not supported.")
                    foundSelfLoop = True
                    outFile.write("  inState"+str(state)+" = inState"+str(state)+" && (")
                    assert condition[0]=="["
                    assert condition[-1]=="]"
                    condition = condition[1:-1]
                    disjuncts = condition.split("|")
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
                    outFile.write(");\n")
            if not foundSelfLoop:
                outFile.write("  inState"+str(state)+" = 0;\n")

            # Then, all other incoming transitions
            for transitionNum,(sourcestate,condition) in enumerate(reverseTransitionRelation[state]):
                if (sourcestate!=state):
                    assert condition[0]=="["
                    assert condition[-1]=="]"
                    condition = condition[1:-1]
                    disjuncts = condition.split("|")
                    outFile.write("  if (inState"+str(sourcestate)+" && (")
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
                    outFile.write(")) {\n    inState"+str(state)+" = 1;\n")
                    # Copy violation tracking information from previous state...
                    outFile.write("    *((state"+str(sourcestate)+"apValuesType*)&state"+str(state)+"apValues) = state"+str(sourcestate)+"apValues;\n")
                    outFile.write("     state"+str(state)+"apValues.vals"+str(maxLengthIncomingTracePerState[state]-1)+" = CONSTRUCTTRACEELEMENT;\n" )
                    outFile.write("     /* Transition from state "+str(sourcestate)+": "+str(condition)+"*/\n")
                    outFile.write("     state"+str(state)+"history = state"+str(sourcestate)+"history * "+str(len(reverseTransitionRelation[state])) + " + "+str(transitionNum)+";\n")
                    outFile.write("  }\n")

        outFile.write("  return inState"+str(rejectingState)+";\n")
        outFile.write("}\n")
        outFile.write("#undef CONSTRUCTTRACEELEMENT\n\n")

        # Write trace decompiler.
        outFile.write("/* Function for obtaining trace information from stored values. Uses statically reserved space */")
        maximumTraceLength = max([b for (a,b) in maxLengthIncomingTracePerState.items()])
        stringLengthOfAllAPs = sum([len(a)+3 for a in APs])
        maxSpaceViolationTrackingPrintout = 64+64*maximumTraceLength+stringLengthOfAllAPs*maximumTraceLength
        outFile.write("char violatingTraceInformation["+str(maxSpaceViolationTrackingPrintout+1)+"];\n")
        outFile.write("void buildViolatingTraceInformation() {\n")
        outFile.write("  int selector;\n")
        outFile.write("  uint32_t history = state"+str(rejectingState)+"history;\n")
        outFile.write("  int state="+str(rejectingState)+";\n")
        outFile.write("  int ptr=snprintf(violatingTraceInformation,"+str(maxSpaceViolationTrackingPrintout)+",\"Violating trace (before entering the rejecting state):\\n\");\n")
        outFile.write("  while ("+"&&".join(["(state!="+str(a)+")" for a in startingStates])+") {\n")
        outFile.write("    switch(state) {\n")
        for state in range(nofStates):
            if not state in startingStates:
                outFile.write("      case "+str(state)+":\n")
                outFile.write("        selector = (int[]){")
                for j,a in enumerate(reverseTransitionRelation[state]):
                    if j>0:
                        outFile.write(",")
                    outFile.write(str(a[0]))
                outFile.write("}[history % "+str(len(reverseTransitionRelation[state]))+"];\n")
                outFile.write("        ptr += snprintf(violatingTraceInformation+ptr,"+str(maxSpaceViolationTrackingPrintout)+"-ptr,\"- Transition from state %d with prop. values: ")
                for i,a in enumerate(APs):
                    if i>0:
                        outFile.write(", ")
                    outFile.write(a+"=%d")
                outFile.write("\\n\",selector")
                src = "state"+str(rejectingState)+"apValues.vals"+str(maxLengthIncomingTracePerState[state]-1)
                for i,a in enumerate(APs):
                    outFile.write(", ")
                    outFile.write("("+src+" & (1 << "+str(i)+"))?1:0")
                outFile.write(");\n")
                outFile.write("        history =  history / "+str(len(reverseTransitionRelation[state]))+";\n")
                outFile.write("        state = selector;\n")
                outFile.write("        break;\n")
        outFile.write("    }\n")              
        outFile.write("  }\n")              
        outFile.write("}\n")


def monitorCompilerUniversalWithReasonTrackingWithoutUVWOptimizations(filenameIn,filenameOut):
    """This version of the translation does not use the fact that the UVW is very weak, which causes additional
    state copying operations."""
    with open(filenameOut,"w") as outFile:
        outFile.write("/* Temporal Logic Runtime Monitor Code */\n")
        outFile.write("#include <stdint.h>\n")
        outFile.write("#include <stdio.h>\n")
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

        # Tracking states works by multiplying the incoming state information by #incoming
        # edges, and then adding the incoming edge number
        # Also store max #transitions leading here
        # Note th
        maxLengthIncomingTracePerState = {a:0 for a in startingStates}
        todo = [a for a in startingStates]
        while len(todo)>0:
            thisOne = todo[0]
            todo = todo[1:]
            thisValue = maxLengthIncomingTracePerState[thisOne]
            for (a,b) in statesToTransitionsMapper[thisOne]:
                if not a in maxLengthIncomingTracePerState:
                    maxLengthIncomingTracePerState[a] = thisValue+1
                    todo.append(a)
                else:
                    if not a==thisOne:
                        if maxLengthIncomingTracePerState[thisOne]<thisValue+1:
                            maxLengthIncomingTracePerState[a] = thisValue+1
                            if not a in todo:
                                todo.append(a)
                            if thisValue>100:
                                raise Exception("Error: The input automaton is not very weak")
        

        # Find rejecting state
        rejectingState = None
        for state in range(nofStates):
            if len(statesToTransitionsMapper[state])==1:
                if statesToTransitionsMapper[state][0][0]==state and statesToTransitionsMapper[state][0][1]=="[t]":
                    assert rejectingState is None
                    rejectingState = state
        assert not rejectingState is None

        # Data type for AP values:
        if len(APs)<=8:
            dataTypeAPValues = "uint8_t"
        else:
            dataTypeAPValues = "uint32_t"
            assert len(APs)<=32

        # Build state information
        outFile.write("/* State storage information */\n")
        for i in range(0,nofStates):
            if i in startingStates:
                outFile.write("uint8_t inState"+str(i)+" = 1;\n")
            else:
                outFile.write("uint8_t inState"+str(i)+" = 0;\n")
            outFile.write("volatile uint32_t state"+str(i)+"history = 0;\n")
            # Backup.
            outFile.write("typedef struct {\n")
            for j in range(maxLengthIncomingTracePerState[i]):
                outFile.write("  "+dataTypeAPValues+"  vals"+str(j)+";\n")
            outFile.write("} state"+str(i)+"apValuesType;\n")
            outFile.write("volatile state"+str(i)+"apValuesType state"+str(i)+"apValues;\n")

        # Construct matro to compress APs to integer data type used for trace
        # storage
        outFile.write("#define CONSTRUCTTRACEELEMENT (")
        for i,a in enumerate(APs):
            if i>0:
                outFile.write("+")
            outFile.write("("+a+">0?"+str(1 << i)+":0)")
        outFile.write(")\n")

        # Build a revserse transition relation, so that we can 
        # access for each state which incoming states there are
        reverseTransitionRelation = [[] for a in statesToTransitionsMapper]
        for (a,b) in statesToTransitionsMapper.items():
            for c in b:
                reverseTransitionRelation[c[0]].append((a,c[1]))

        # Check assumption that there is no incoming edge in an initial state that
        # is not a self-loop -- the debug trace printer below doesn't work otherwise.
        for a in startingStates:
            for (b,c) in reverseTransitionRelation[a]:
                assert b==a

        # Build a topological sort of the depth in the weak automaton, so that
        # we can go leaf-to-root in the construction
        reverseTopologyPre = {}
        for (a,b) in maxLengthIncomingTracePerState.items():
            if not b in reverseTopologyPre:
                reverseTopologyPre[b] = [a]
            else:
                reverseTopologyPre[b].append(a)
        allLengths = list(set([a for (a,b) in reverseTopologyPre.items()]))
        allLengths.sort(reverse=True)
        topologicalStateSort = []
        for a in allLengths:
            topologicalStateSort.extend(reverseTopologyPre[a])
        
        # Build transition function
        # Use topological sorting, so that "nextState" variables are not needed.
        outFile.write("\n/* Monitor step/update function */\n")
        outFile.write("int monitor("+",".join(["uint8_t "+a for a in APs])+") {\n")
        for state in topologicalStateSort:
            outFile.write("  uint8_t nextState"+str(state)+" = 0;\n")

        for state in topologicalStateSort:
            # First: process self-loops
            foundSelfLoop = False
            for (target,condition) in statesToTransitionsMapper[state]:
                if (target==state):
                    if foundSelfLoop:
                        raise Exception("Error: Multiple self-loops are not supported.")
                    foundSelfLoop = True
                    outFile.write("  nextState"+str(state)+" = inState"+str(state)+" && (")
                    assert condition[0]=="["
                    assert condition[-1]=="]"
                    condition = condition[1:-1]
                    disjuncts = condition.split("|")
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
                    outFile.write(");\n")
            # if not foundSelfLoop:
            #     outFile.write("  inState"+str(state)+" = 0;\n")

            # Then, all other incoming transitions
            for transitionNum,(sourcestate,condition) in enumerate(reverseTransitionRelation[state]):
                if (sourcestate!=state):
                    assert condition[0]=="["
                    assert condition[-1]=="]"
                    condition = condition[1:-1]
                    disjuncts = condition.split("|")
                    outFile.write("  if (inState"+str(sourcestate)+" && (")
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
                    outFile.write(")) {\n    nextState"+str(state)+" = 1;\n")
                    # Copy violation tracking information from previous state...
                    outFile.write("    *((state"+str(sourcestate)+"apValuesType*)&state"+str(state)+"apValues) = state"+str(sourcestate)+"apValues;\n")
                    outFile.write("     state"+str(state)+"apValues.vals"+str(maxLengthIncomingTracePerState[state]-1)+" = CONSTRUCTTRACEELEMENT;\n" )
                    outFile.write("     /* Transition from state "+str(sourcestate)+": "+str(condition)+"*/\n")
                    outFile.write("     state"+str(state)+"history = state"+str(sourcestate)+"history * "+str(len(reverseTransitionRelation[state])) + " + "+str(transitionNum)+";\n")
                    outFile.write("  }\n")

        # Copy values back
        for i in topologicalStateSort:
            outFile.write("  inState"+str(i)+" = nextState"+str(i)+";\n")

        outFile.write("  return inState"+str(rejectingState)+";\n")
        outFile.write("}\n")
        outFile.write("#undef CONSTRUCTTRACEELEMENT\n\n")

        # Write trace decompiler.
        outFile.write("/* Function for obtaining trace information from stored values. Uses statically reserved space */")
        maximumTraceLength = max([b for (a,b) in maxLengthIncomingTracePerState.items()])
        stringLengthOfAllAPs = sum([len(a)+3 for a in APs])
        maxSpaceViolationTrackingPrintout = 64+64*maximumTraceLength+stringLengthOfAllAPs*maximumTraceLength
        outFile.write("char violatingTraceInformation["+str(maxSpaceViolationTrackingPrintout+1)+"];\n")
        outFile.write("void buildViolatingTraceInformation() {\n")
        outFile.write("  int selector;\n")
        outFile.write("  uint32_t history = state"+str(rejectingState)+"history;\n")
        outFile.write("  int state="+str(rejectingState)+";\n")
        outFile.write("  int ptr=snprintf(violatingTraceInformation,"+str(maxSpaceViolationTrackingPrintout)+",\"Violating trace (before entering the rejecting state):\\n\");\n")
        outFile.write("  while ("+"&&".join(["(state!="+str(a)+")" for a in startingStates])+") {\n")
        outFile.write("    switch(state) {\n")
        for state in range(nofStates):
            if not state in startingStates:
                outFile.write("      case "+str(state)+":\n")
                outFile.write("        selector = (int[]){")
                for j,a in enumerate(reverseTransitionRelation[state]):
                    if j>0:
                        outFile.write(",")
                    outFile.write(str(a[0]))
                outFile.write("}[history % "+str(len(reverseTransitionRelation[state]))+"];\n")
                outFile.write("        ptr += snprintf(violatingTraceInformation+ptr,"+str(maxSpaceViolationTrackingPrintout)+"-ptr,\"- Transition from state %d with prop. values: ")
                for i,a in enumerate(APs):
                    if i>0:
                        outFile.write(", ")
                    outFile.write(a+"=%d")
                outFile.write("\\n\",selector")
                src = "state"+str(rejectingState)+"apValues.vals"+str(maxLengthIncomingTracePerState[state]-1)
                for i,a in enumerate(APs):
                    outFile.write(", ")
                    outFile.write("("+src+" & (1 << "+str(i)+"))?1:0")
                outFile.write(");\n")
                outFile.write("        history =  history / "+str(len(reverseTransitionRelation[state]))+";\n")
                outFile.write("        state = selector;\n")
                outFile.write("        break;\n")
        outFile.write("    }\n")              
        outFile.write("  }\n")              
        outFile.write("}\n")

    
# Preparation for c<ses where all specification blocks are merged
specLinesOfAllBlocks = []
for a in specBlocks:
    specLinesOfAllBlocks.extend(a)
specOfAllBlocks = " & ".join(["("+a+")" for a in specLinesOfAllBlocks])

    
# Experiment: Universal automaton
assert os.system("lib/spot-2.12/bin/ltl2tgba -f \"!("+specOfAllBlocks+")\" > results/uca.txt")==0
assert os.system("lib/spot-2.12/bin/ltl2tgba -f \"!("+specOfAllBlocks+")\" -d > results/uca.dot")==0

for version in [0,1]:
    if version==0:
        monitorCompilerUniversalWithReasonTracking("results/uca.txt","results/uca_reason_tracking.c")
        print("======================With UVW optimizations====================")
    else:
        monitorCompilerUniversalWithReasonTrackingWithoutUVWOptimizations("results/uca.txt","results/uca_reason_tracking.c")
        print("======================Without UVW optimizations====================")
    assert os.system("cp results/uca_reason_tracking.c pioproject/src/monitor.c")==0
    assert os.system("cd pioproject; pio run > /tmp/pioout 2>&1")==0
    (newRAM,newFlash) = getSizesForF446RE("/tmp/pioout")
    if USE_NUCLEO_BOARD:
        assert os.system("cd pioproject; pio run --target upload > /tmp/pioout2 2>&1")==0
        port = serial.Serial(glob.glob("/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink*")[0],baudrate=115200)
        nofCyclesThis = port.readline().strip().decode ("utf-8")
        port.close()
        if not nofCyclesThis.startswith("#Cycles:"):
            print("Illegal starting line from the serial line: ",nofCyclesThis)
            sys.exit(1)
        nofCyclesThis = int(nofCyclesThis.split(" ")[1])
        
    # print results
    print("Results:")
    print("(Base Flash/RAM/Clock cycle usage not yet subtracted -- the 'build_monitors.py' script outputs these values to the terminal when running)")
    print("RAM used:",newRAM)
    print("Flash used:",newFlash)
    if USE_NUCLEO_BOARD:
        print("#Cycles -- divide by 7 after subtracting the base number: ",nofCyclesThis)
