Data package for a paper at the TAP 2024 
========================================
This data package details result of the paper "Efficient Temporal Logic Runtime Monitoring for Tiny Systems", published at TAP 2024.

The results reported on are contained in the folder "results" and in the file "resultsTexMacros.tex". They can be looked at directly.

Alternatively, the experiments can be re-run, and the files in the "result" folder as well as the results in "resultsTexMacros.tex" are rebuilt then.

In case there will be post-archival updates to the data package, they will be made available at: https://github.com/tuc-es/TemporalLogicMonitoringForTinySystemsCaseStudy

License
-------
The content of this data package is licensed under CC BY-SA version 4.0, available at https://creativecommons.org/licenses/by-sa/4.0/legalcode

The "build_monitors.py" script is dual-licensed under a BSD license, so it can be freely built on. A license text for the BSD license is part of the script.


Overview
========
The data package containts the scripts, data, and results for three types of experiments, namely:

1. Experiments on software monitors for a running example,
2. An experiment on a hardware monitor encoding of the example used for comparing the different types of software monitors, and
3. A monitoring example for using a monitor that track the reason for a violation

The experiments of these three types need to be run separately, and how to do so is explained in the following.


Preparation / Requirements
--------------------------
A modern version of Linux is assumed in the following.

Then, go the the lib directory and run "./get\_libs.sh" from there. This will download "spot" and compile it. You will need a suitable compilation environment installed for this purpose. This script also clones a copy of the monitor compiler tool.


Running the experiments on the software monitors
------------------------------------------------
To prepare running the experiments, first install platformio (see https://docs.platformio.org/en/latest/core/installation/index.html) and the Python 3 package pyserial.

Then, edit "./build_monitors.py" program to select in the line starting with ´´USE\_NUCLEO\_BOARD´´ if you want to run the code on a the ST Microelectronics Nucleo64-F446RE board, which needs to be attached via USB. While this board is easy to obtain via mail order with multiple vendors (don't forget ordering a USB->Mini USB cable so you can actually connected the board!), most researchers who want to rerun the experiments probably do not want to obtain one, and want to select "False" here. For computing the size of the monitor program code, the board is not needed, only for measuring the computation times of the monitors.

Afterwards, run "./build_monitors.py". The files in the "results" folder as well as "resultsTexMacros.tex" are rewritten.

To test all generated monitors, you can run "./test\_non\_tracking.sh" in the "monitortest" folder. You will need to have clang installed to compile and run the monitors.


Running the experiments for the hardware-based monitor
------------------------------------------------------
The specification from the paper has been manually encoded into the input of a monitor compiler tool. Running "./run\_this\_experiment.sh" in the "monitor\_for\_caravel" directory builds program code to be used in a program to be run on the caravel SoC. The numbers reported in the paper were read off manually from the output.


Experiment on Violation Tracking Monitor
----------------------------------------
To build a violation-tracking monitor (using universal automata), you can run "./build\_violation\_reason\_tracking\_monitor.py" from the main directory. This will build the "results/uca\_reason\_tracking.c" monitor implementation. You can test it (locally on your computer) by running the "test\_non\_tracking.sh" script.

