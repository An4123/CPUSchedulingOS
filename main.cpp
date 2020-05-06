// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//
// Comments:

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "readyQueue.h"
using namespace std;

// declration of functions

int findNumProc(string);
void initilizeAllProcess(string, process[]);
void FCFS(process [],int, string);
void SRTF(process [],int, string);
void RR(process[],int,int,string);
void PP(process[],int,string);
void printAndOutput(process[],int,int, int, string);

int main(int argc, char* argv[]) {


    string inputFile;
    string outputFile;
    int typeOfAlgorithm;

    int quantumTime = 0;

// reading parameter via command line

    inputFile = string(argv[1]);
    outputFile = string(argv[2]);
    typeOfAlgorithm = atoi(argv[3]);

// finding number of process
    int numberOfProcess = findNumProc(inputFile);

// crearing the array of all process need to processed.
    process allProcess[numberOfProcess];

// if algorithm is Round Robin we need quantum time
    if (typeOfAlgorithm == 2) {
        if (argv[4]) {
            quantumTime = atoi(argv[4]);
        } else {
            cout << "quantum time is needed to run the round robin algorithm\n";
            quantumTime = -100;
        }
    }

// exit if quantum is not provided
    if (quantumTime == -100) {
        return 0;
    } else { // call functions accordingly
        initilizeAllProcess(inputFile, allProcess);
        switch (typeOfAlgorithm) {
            case 0:
                FCFS(allProcess, numberOfProcess, outputFile);
                break;
            case 1:
                SRTF(allProcess, numberOfProcess, outputFile);
                break;
            case 2:
                RR(allProcess, numberOfProcess, quantumTime, outputFile);
                break;
            case 3:
                PP(allProcess, numberOfProcess, outputFile);
                break;
        }

    }
}

int findNumProc(string inputFile){
    int numOfProcesses = 0;
    string currentLine;
    ifstream infile(inputFile);

// just counting number of line in input file
    while (getline(infile, currentLine))
        ++numOfProcesses;
    infile.close();
    return numOfProcesses;
}

void initilizeAllProcess(string inputFile, process allProcess[]){

// for reading line
    int processData[4];
    string line;

// open input file and create input stream
    ifstream infile(inputFile);

// process index
    int allProcessIndex = 0;

// while reading file
    while (getline(infile,line)){

// feeding line to stringstram
// now it will spit as we want
        stringstream ss (line);
        int n;
        for (int i = 0; i < 4; ++i) {
            ss >> n;
            processData[i]=n;
        }

// create temp process

        process temp(processData);


// assigning it to process array

        allProcess[allProcessIndex]= temp;

// increasing index
        ++allProcessIndex;
    }
}

void FCFS(process allProcess[],int numOfprocess, string outFile) {
    float time = 0; // system time
    readyQueue rq; // creating queue
    int noOfRemainingProc = numOfprocess; // num of process that are not finished
// int totalNoOfContextSwitch = 0;

    process *CPUprocess = nullptr; // process currently in CPU
    bool procInCPU = false; // is CPU ready to take next process?

// as long as there are not finished proc left

    while (noOfRemainingProc != 0) {
        for (int i = 0; i < numOfprocess; ++i) {

// if process from allprocess is not finished, is not in ready queue and arrival time earlier than clock
            if (!allProcess[i].finished && !rq.procExist(allProcess[i]) &&
                (time >= allProcess[i].getArrivalTime()) && (&allProcess[i] != CPUprocess)) {
                rq.push(&allProcess[i]);
            }
        }

// if these is no process in CPU let us send one
// and pop it from ready queue
        if (!procInCPU) {
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();
        }

// if this process is going to the cpu first time lets calculate response time
        if (CPUprocess->firstTimeInCPU) {
            CPUprocess->responseTime = time - CPUprocess->getArrivalTime();
            CPUprocess->firstTimeInCPU = false;
        }

// increse waiting time for rest of process which are in queue
        for (int j = 0; j < rq.size; ++j) {
            ++rq.myQueue[j]->waitingTime;
        }

// decrement remaining CPU for current process
        --CPUprocess->remainingCPU;

// calculate and assign everything if process is complete.
        if (CPUprocess->remainingCPU == 0) {
            CPUprocess->finishTime = time + 1;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->getCPUburstTime() + CPUprocess->waitingTime;

//now there is no proc in CPU
            procInCPU = false;

// total remaining process is also decreased.
            --noOfRemainingProc;
        }
//system time increasing
        ++time;
    }

// print to console and file (-100) is centinel to tell we don't have quantum
    printAndOutput(allProcess,numOfprocess,0,-100,outFile);
}


void SRTF(process allProcess[],int numOfprocess, string outFile){
    float time = 0; // system time
    readyQueue rq; // creating queue
    int noOfRemainingProc = numOfprocess; // num of process that are not finished


    process *CPUprocess = nullptr;
    bool procInCPU = false;

// as long as there are not finished proc left

    while (noOfRemainingProc != 0) {
        for (int i = 0; i < numOfprocess; ++i) {

// if process from allprocess is not finished, is not in ready queue and arrival time earlier than clock
// also we don't want to push the process in ready queue if it is in CPU
            if(!allProcess[i].finished && !rq.procExist(allProcess[i]) &&(time >= allProcess[i].getArrivalTime()) && (&allProcess[i] != CPUprocess) ){
                rq.push(&allProcess[i]);

// some process may be waiting when we were busy context switching
// lets add that on those process
                if(time > allProcess[i].getArrivalTime()){
                    allProcess[i].waitingTime +=(time - allProcess[i].getArrivalTime());
                }
            }
        }

// if there is no process in CPU give one and pop from ready queue
        if(!procInCPU) {
            rq.sortByTime();
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();
        } else{ // lets check if there are process which have less remaining time than current one
            rq.sortByTime();
            if (rq.size > 0) { // if there are any process in queue we have to go further

// swapping happening here
// and context switching cost also added to every process
                if(CPUprocess->remainingCPU > rq.myQueue[0]->remainingCPU){
                    rq.push(CPUprocess);
                    ++CPUprocess->contextSwitchNumber;
                    CPUprocess = rq.myQueue[0];
                    rq.pop();
                    rq.sortByTime();
                    CPUprocess->waitingTime += 0.5;
                    for (int i = 0; i < rq.size; ++i) {
                        rq.myQueue[i]->waitingTime += 0.5;
                    }
                    time += 0.5; // increase system time.
                }
            }
        }

// if this process is going to the cpu first time lets calculate response time
        if( CPUprocess->firstTimeInCPU){
            CPUprocess->responseTime = time -CPUprocess->getArrivalTime();
            CPUprocess->firstTimeInCPU = false;
        }

// increse waiting time for all process in queue
        for (int j = 0; j < rq.size; ++j) {
            ++rq.myQueue[j]->waitingTime;
        }

// remaining time is decreasing
        --CPUprocess->remainingCPU;

// calculate and assign everything if process is complete.
        if(CPUprocess->remainingCPU == 0){
            CPUprocess->finishTime = time+1;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->getCPUburstTime()+CPUprocess->waitingTime;
            procInCPU = false;
            --noOfRemainingProc;
        }
        ++time;
    }
// print to console and file (-100) is centinel to tell we don't have quantum
    printAndOutput(allProcess,numOfprocess,1,-100,outFile);

}


void RR(process allProcess[],int numOfprocess, int quantum, string outFile){
    float time = 0; // system time
    readyQueue rq; // creating queue
    int noOfRemainingProc = numOfprocess; // num of process that are not finished
// int totalNoOfContextSwitch = 0;

    process *CPUprocess = nullptr;
    bool procInCPU = false;

// just for debugging

// as long as there are not finished proc left

    while (noOfRemainingProc != 0) {
        for (int i = 0; i < numOfprocess; ++i) {

// if process from allprocess is not finished, is not in ready queue and arrival time earlier than clock
// also we don't want to push the process in ready queue if it is in CPU
            if(!allProcess[i].finished && !rq.procExist(allProcess[i]) &&(time >= allProcess[i].getArrivalTime()) && (&allProcess[i] != CPUprocess) ){
                rq.push(&allProcess[i]);
                if(time > allProcess[i].getArrivalTime()){
                    allProcess[i].waitingTime +=(time - allProcess[i].getArrivalTime());
                }
            }
        }
// if there is no process in CPU give one and pop from ready queue
        if(!procInCPU) {
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();
        }else if (rq.size > 0) { // only swap will happen if there is other process available
            rq.push(CPUprocess);
            ++CPUprocess->contextSwitchNumber;
            CPUprocess = rq.myQueue[0];
            rq.pop();
            CPUprocess->waitingTime += 0.5;
            for (int i = 0; i < rq.size; ++i) {
                rq.myQueue[i]->waitingTime += 0.5;
            }
            time += 0.5;
        }


// if this process is going to the cpu first time lets calculate response time
        if( CPUprocess->firstTimeInCPU){
            CPUprocess->responseTime = time -CPUprocess->getArrivalTime();
            CPUprocess->firstTimeInCPU = false;
        }

// if reaming time is more than quantum decrease time and kick the process out for next one.
        if(CPUprocess->remainingCPU > quantum){
            CPUprocess->remainingCPU -= quantum;
            time +=quantum;
            for (int j = 0; j < rq.size; ++j) {
                rq.myQueue[j]->waitingTime += quantum;
            }

        } else{
            time +=CPUprocess->remainingCPU;
            for (int j = 0; j < rq.size; ++j) {
                rq.myQueue[j]->waitingTime +=CPUprocess->remainingCPU;
            }
// calculate and assign everything since process is complete.
            CPUprocess->finishTime = time;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->getCPUburstTime()+CPUprocess->waitingTime;
            procInCPU = false;
            --noOfRemainingProc;
        }

    }

// print to console and file
    printAndOutput(allProcess,numOfprocess,2,quantum,outFile);
}

void PP(process allProcess[],int numOfprocess, string outFile) {
    float time = 0; // system time
    readyQueue rq; // creating queue
    int noOfRemainingProc = numOfprocess; // num of process that are not finished
// int totalNoOfContextSwitch = 0;

    process *CPUprocess = nullptr;
    bool procInCPU = false;

// just for debugging

// as long as there are not finished proc left

    while (noOfRemainingProc != 0) {
        for (int i = 0; i < numOfprocess; ++i) {

// if process from allprocess is not finished, is not in ready queue and arrival time earlier than clock
// also we don't want to push the process in ready queue if it is in CPU
            if (!allProcess[i].finished && !rq.procExist(allProcess[i]) && (time >= allProcess[i].getArrivalTime()) &&
                (&allProcess[i] != CPUprocess)) {
                rq.push(&allProcess[i]);
                if (time > allProcess[i].getArrivalTime()) {
                    allProcess[i].waitingTime += (time - allProcess[i].getArrivalTime());
                }
            }
        }
// if there is no process in CPU give one and pop from ready queue
        if (!procInCPU) {
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();
        } else { // if there are process in queue and they are higher priority
// swap will occur
            rq.sortByPriority();
            if (rq.size > 0) {
                if (CPUprocess->getPriority() > rq.myQueue[0]->getPriority()) {
                    rq.push(CPUprocess);
                    ++CPUprocess->contextSwitchNumber;
                    CPUprocess = rq.myQueue[0];
                    rq.pop();
                    rq.sortByPriority();
                    CPUprocess->waitingTime += 0.5;
                    for (int i = 0; i < rq.size; ++i) {
                        rq.myQueue[i]->waitingTime += 0.5;
                    }
                    time += 0.5;
                }

            }
        }

// if this process is going to the cpu first time lets calculate response time
        if (CPUprocess->firstTimeInCPU) {
            CPUprocess->responseTime = time - CPUprocess->getArrivalTime();
            CPUprocess->firstTimeInCPU = false;
        }

// increase waiting time for all all process in queue

        for (int j = 0; j < rq.size; ++j) {
            ++rq.myQueue[j]->waitingTime;
        }


        --CPUprocess->remainingCPU;

// calculate and assign everything if process is complete.
        if (CPUprocess->remainingCPU == 0) {
            CPUprocess->finishTime = time + 1;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->getCPUburstTime() + CPUprocess->waitingTime;
            procInCPU = false;
            --noOfRemainingProc;
        }
        ++time;

    }

// print to console and file (-100) is centinel to tell we don't have quantum
    printAndOutput(allProcess,numOfprocess,3,-100,outFile);
}

void printAndOutput(process allProcess[],int numOfprocess,int typeOfAlgorithm, int quantum, string outFile){

    ofstream outPut(outFile);
    string algorithmName;
    float totalCPUtime = 0;
    float totalTurnAroundTime = 0;
    float totalWaitingTime = 0;
    float totalResponseTime = 0;
    int totalContextSwitch = 0;


    if(typeOfAlgorithm == 0){
        algorithmName = "FCFS *****************";
    }
    else if(typeOfAlgorithm == 1){
        algorithmName = "SRTF *****************";
    }
    else if(typeOfAlgorithm == 2){
        algorithmName = "Round Robin **********";
    }
    else if(typeOfAlgorithm == 3){
        algorithmName = "Preemptive Priority **";
    }


    cout << string(90,'*') << endl;
    cout << string(24,'*') << " Scheduling algorithm: " << algorithmName << string(21,'*') << endl;
    if(quantum != -100){
        cout << string(24,'*') << " ( No. of Task = : "<< numOfprocess << " Quantum = " << quantum << " ) " << string(31,'*') << endl;
    }
    cout << string(90,'*') << endl << endl;

    cout << " PID " << " Arrival "<< " CPU-Burst "<< " Priority "<< " Finish "
         << " Waiting-Time "<< " Turnaround "<< " Response-Time " << " No.of Context-Switch \n";

    for (int i = 0; i < numOfprocess; ++i) {
        cout << fixed << setprecision(1)
             << " " << setw(5) << left << allProcess[i].getPID()
             << " " << setw(7) << left << allProcess[i].getArrivalTime()
             << " " << setw(8) << left << allProcess[i].getCPUburstTime()
             << " " << setw(9) << left << allProcess[i].getPriority()
             << " " << setw(7) << left << allProcess[i].finishTime
             << " " << setw(12) << left << allProcess[i].waitingTime
             << " " << setw(10) << left << allProcess[i].turnAroundTime
             << " " << setw(15) << left << allProcess[i].responseTime
             << " " << allProcess[i].contextSwitchNumber
             << " " << endl;

        totalCPUtime += allProcess[i].getCPUburstTime();
        totalWaitingTime += allProcess[i].waitingTime;
        totalTurnAroundTime += allProcess[i].turnAroundTime;
        totalResponseTime += allProcess[i].responseTime;
        totalContextSwitch += allProcess[i].contextSwitchNumber;
    }


    cout << fixed << setprecision(2)
         << "\nAverage CPU burst time = " << totalCPUtime/numOfprocess << " ms, "
         << "Average waiting time = " << totalWaitingTime/numOfprocess << " ms\n"
         << "Average turn-around time = " << totalTurnAroundTime/numOfprocess << " ms, "
         << setprecision(3) << "Average response time = " << totalResponseTime/numOfprocess << " ms\n"
         << "Total No. of Context Switching Performed = " << totalContextSwitch << endl;

// below will be output to file


    outPut << string(90,'*') << endl;
    outPut << string(24,'*') << " Scheduling algorithm: " << algorithmName << string(21,'*') << endl;
    if(quantum != -100){
        outPut << string(24,'*') << " ( No. of Task = : "<< numOfprocess << " Quantum = " << quantum << " ) " << string(31,'*') << endl;
    }
    outPut << string(90,'*') << endl << endl;

    outPut << " PID " << " Arrival "<< " CPU-Burst "<< " Priority "<< " Finish "
           << " Waiting-Time "<< " Turnaround "<< " Response-Time " << " No.of Context-Switch \n";

    for (int i = 0; i < numOfprocess; ++i) {
        outPut << fixed << setprecision(1)
               << " " << setw(5) << left << allProcess[i].getPID()
               << " " << setw(7) << left << allProcess[i].getArrivalTime()
               << " " << setw(8) << left << allProcess[i].getCPUburstTime()
               << " " << setw(9) << left << allProcess[i].getPriority()
               << " " << setw(7) << left << allProcess[i].finishTime
               << " " << setw(12) << left << allProcess[i].waitingTime
               << " " << setw(10) << left << allProcess[i].turnAroundTime
               << " " << setw(15) << left << allProcess[i].responseTime
               << " " << allProcess[i].contextSwitchNumber
               << " " << endl;
    }


    outPut << fixed << setprecision(2)
           << "\nAverage CPU burst time = " << totalCPUtime/numOfprocess << " ms, "
           << "Average waiting time = " << totalWaitingTime/numOfprocess << " ms\n"
           << "Average turn-around time = " << totalTurnAroundTime/numOfprocess << " ms, "
           << setprecision(3) << "Average response time = " << totalResponseTime/numOfprocess << " ms\n"
           << "Total No. of Context Switching Performed = " << totalContextSwitch << endl;

}
