Data package for a paper at the TAP 2024 
========================================
This data package details result of the paper "Efficient Temporal Logic Runtime Monitoring for Tiny Systems", published at TAP 2024.

The results reported on are contained in the folder "results" and in the file "resultsTexMacros.tex". They can be looked at directly.

Alternatively, the experiments can be re-run.

In case there will be post-archival updates to the data package, they will be made available at: https://github.com/tuc-es/TemporalLogicMonitoringForTinySystemsCaseStudy

License
-------
The content of this data package is licensed under CC BY-SA version 4.0, available at https://creativecommons.org/licenses/by-sa/4.0/legalcode

The "build_monitors.py" script is dual-licensed under a BSD license, so it can be freely built on. A license text is part of the script.



Running the experiments on the software monitors
------------------------------------------------
A modern version of Linux is assumed in the following.

To prepare running the experiments, first install platformio (see https://docs.platformio.org/en/latest/core/installation/index.html) and the python package pyserial.

Then, go the the lib directory and run "./get_libs.sh" from there. This will download "spot" and compile it.

Then, edit "./build_monitors.py" program to select in the line starting with "USE_NUCLEO_BOARD" if you want to run the code on a the ST Microelectronics Nucleo64-F446RE board, which would need to be attached via USB. While this board is easy to obtain via mail order with multiple vendors (don't forget ordering a USB->Mini USB cable!), most researchers who want to rerun the experiments probably do not have one, and want to select "False" here. For computing the size of the monitor program code, the board is not needed, only for measuring the computation times of the monitors.

Afterwards, run "./build_monitors.py". The files in the "results" folder as well as "resultsTexMacros.tex" are rewritten.



Running the experiments for the hardware-based monitor
------------------------------------------------------
The specification from the paper has been manually encoded into the input of a monitor compiler tool. Running "./run_this_experiment.sh" in the "monitor_for_caravel" directory builds program code to be used in a program to be run on the caravel SoC. The numbers reported in the paper were read off manually from the output.




Additional numbers in the paper
-------------------------------
Why 130 extra bytes of RAM + 12 bytes for storing the trace informatioN?
