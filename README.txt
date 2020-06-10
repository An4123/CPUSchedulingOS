Name: An Vo
Class: Intro to Operating Systems
Project: Lab3

HOW TO COMPILE/RUN
- unZip file.
- Run Terminal.
- cd into unzipped file.
- run 'make' (without quotes)
- run './VoLab3 <inputFileName> <Algorithm> <QuantumTime>'
	- Algorithms: FCFS, SRTF, RR
	- Quantumtimes: Optional

COMMENTS: 
- I got some help online help with ideas on how to tackle this lab and where to start
  but I seem to have lost the link to it.
- I understand the code fully and the comments are written by me to show I understand this lab. 

ORGANIZATION:
---main.cpp---
- Gets user arguments from command line and determines what algorithm to do.

---Functions.h---
- Runs the algorithms
- Finds number of Processes
- Prints the wanted information

---readyQueue.h/.cpp---
- Class to set up a Queue
- Contains an array of processes, size of queue, and queue manipulation functions.

---process.h/.cpp---
- Class for process object
- Each process contains:
	- PID, arrivalTime, CPUburstTime, and other variables to keep track of processes as
	  it is being handled by CPU.

STATUS:
- Every algorithm works.
- Tested on CSEGRID and my linux computer
- No known bugs 

SOFT/HARDWARE:
-Tested on:
	- CSEGRID
	- Personal Linux Device

