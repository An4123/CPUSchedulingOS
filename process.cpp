// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//

#include "process.h"

process::process() {
    PID = 0;
    arrivalTime = 0;
    CPUburstTime = 0;
    remainingCPU = CPUburstTime;
    contextSwitchNumber = 0;
    finished = false;
    finishTime = 0;
    waitingTime = 0;
    turnAroundTime = 0;

}

process::process(int * data) {

    PID = data[0];
    arrivalTime = data[1];
    CPUburstTime = data[2];
    remainingCPU = CPUburstTime;
    contextSwitchNumber = 0;
    finished = false;
    finishTime = 0;
    waitingTime = 0;
    turnAroundTime = 0;
}

