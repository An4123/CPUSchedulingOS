//
// Created by super on 5/6/2020.
//

#include "process.h"

process::process() {
    PID = 0;
    arrivalTime = 0;
    CPUburstTime = 0;
    priority = 0;
    remainingCPU = CPUburstTime;
    contextSwitchNumber = 0;
    finished = false;
    firstTimeInCPU = true;
    responseTime = 0;
    finishTime = 0;
    waitingTime = 0;
    turnAroundTime = 0;

}

process::process(int * data) {

    PID = data[0];
    arrivalTime = data[1];
    CPUburstTime = data[2];
    priority = data[3];
    remainingCPU = CPUburstTime;
    contextSwitchNumber = 0;
    finished = false;
    firstTimeInCPU = true;
    responseTime = 0;
    finishTime = 0;
    waitingTime = 0;
    turnAroundTime = 0;
}

int process::getPID() {
    return PID;
}

int process::getArrivalTime() {
    return arrivalTime;
}

int process::getCPUburstTime() {
    return CPUburstTime;
}

int process::getPriority() {
    return priority;
}