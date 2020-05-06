// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//

#include <sstream>
#include <iomanip>
#include <fstream>
#include "process.h"
#include "readyQueue.h"
using namespace std;
void printAndOutput(process allProcess[],int numOfprocess,int typeOfAlgorithm, int quantum);

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
    int processData[4];
    string line;
    ifstream infile(inputFile);             // get input from file
    int allProcessIndex = 0;                // iterator to iterate the processes

    while (getline(infile,line)){           // while reading file
        stringstream ss (line);                     // put line as string stream
        int n;
        for(int i = 0; i < 4; ++i) {               // Process the data
            ss >> n;
            processData[i]=n;
        }
        process temp(processData);                  // create a temp process
        allProcess[allProcessIndex]= temp;          // put into process array
        ++allProcessIndex;                          // increase the iterator
    }
}

void FCFS(process allProcess[],int numOfprocess) {
    float time = 0;                                 // system time
    readyQueue rq;                                  // creating queue
    int noOfRemainingProc = numOfprocess;           // num of process not finished

    process *CPUprocess = nullptr;                   // process currently in CPU
    bool procInCPU = false;                         // is CPU ready to take next process?


    while (noOfRemainingProc != 0) {                // while there are still processes
        for (int i = 0; i < numOfprocess; ++i) {
            if (!allProcess[i].finished && !rq.procExist(allProcess[i]) &&     // keep going
                (time >= allProcess[i].arrivalTime) && (&allProcess[i] != CPUprocess)) {
                rq.push(&allProcess[i]);
            }
        }

        if (!procInCPU) {                       // if there is no more processes in CPU
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();                           // remove the last on in readyQueue
        }


        for (int j = 0; j < rq.size; ++j) {      // Increase waiting time for the rest of the processes in Queue
            ++rq.myQueue[j]->waitingTime;
        }

        --CPUprocess->remainingCPU;             // Decrease remaining CPU for current process


        if (CPUprocess->remainingCPU == 0) {    // assign values
            CPUprocess->finishTime = time + 1;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->CPUburstTime + CPUprocess->waitingTime;

            procInCPU = false;           // No more processes in CPU
            --noOfRemainingProc;         // Decrease processes remaining
        }
        ++time;                           // increase system time
    }
    printAndOutput(allProcess,numOfprocess,0,-100);     // print info
}


void SRTF(process allProcess[],int numOfprocess){
    float time = 0;                             // system time
    readyQueue rq;                              // creating queue
    int noOfRemainingProc = numOfprocess;       // num of process that are not finished


    process *CPUprocess = nullptr;
    bool procInCPU = false;

    while (noOfRemainingProc != 0) {                    // while there are still processes in queue
        for (int i = 0; i < numOfprocess; ++i) {
            if(!allProcess[i].finished && !rq.procExist(allProcess[i]) &&(time >= allProcess[i].arrivalTime) && (&allProcess[i] != CPUprocess) ){
                rq.push(&allProcess[i]);
                if(time > allProcess[i].arrivalTime){
                    allProcess[i].waitingTime +=(time - allProcess[i].arrivalTime);
                }
            }
        }
        if(!procInCPU) {            // if there is no more processes
            rq.sortByTime();
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();               // delete the last one
        } else{                     // check if there are any more processes with processes time that is less than the one that is next
            rq.sortByTime();
            if (rq.size > 0) {       // if there is we have to keep going


                if(CPUprocess->remainingCPU > rq.myQueue[0]->remainingCPU){         // if the a differnt process is greater than the one now
                    rq.push(CPUprocess);
                    ++CPUprocess->contextSwitchNumber;
                    CPUprocess = rq.myQueue[0];
                    rq.pop();                                                   // swap them
                    rq.sortByTime();                                            // sort queue
                    CPUprocess->waitingTime += 0.5;                             // increase waiting tim
                    for (int i = 0; i < rq.size; ++i) {
                        rq.myQueue[i]->waitingTime += 0.5;                      // increase waiting time
                    }
                    time += 0.5;                                                // increase system time.
                }
            }
        }


        for (int j = 0; j < rq.size; ++j) {         // increase waiting time for all prcessses still waiting
            ++rq.myQueue[j]->waitingTime;
        }

        --CPUprocess->remainingCPU;         // Decrease remaining Time

        if(CPUprocess->remainingCPU == 0){              // assign these values
            CPUprocess->finishTime = time+1;
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->arrivalTime+CPUprocess->waitingTime;
            procInCPU = false;
            --noOfRemainingProc;
        }
        ++time;
    }
    printAndOutput(allProcess,numOfprocess,1,-100);  // Print out info
}


void RR(process allProcess[],int numOfprocess, int quantum){
    float time = 0;                             // system time
    readyQueue rq;                              // creating queue
    int noOfRemainingProc = numOfprocess;       // num of process that are not finished

    process *CPUprocess = nullptr;
    bool procInCPU = false;

    while (noOfRemainingProc != 0) {                    // Same as above
        for (int i = 0; i < numOfprocess; ++i) {        // SAME process as above
            if(!allProcess[i].finished && !rq.procExist(allProcess[i]) &&(time >= allProcess[i].arrivalTime) && (&allProcess[i] != CPUprocess) ){
                rq.push(&allProcess[i]);
                if(time > allProcess[i].arrivalTime){
                    allProcess[i].waitingTime +=(time - allProcess[i].arrivalTime);
                }
            }
        }

        if(!procInCPU) {                    // same as above
            CPUprocess = rq.myQueue[0];
            procInCPU = true;
            rq.pop();
        }else if (rq.size > 0) {                // only swtich if available
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

        if(CPUprocess->remainingCPU > quantum){                 // if remaining time is > than quantum decrease time
            CPUprocess->remainingCPU -= quantum;
            time +=quantum;
            for (int j = 0; j < rq.size; ++j) {
                rq.myQueue[j]->waitingTime += quantum;         // do the next process instead
            }

        } else{
            time +=CPUprocess->remainingCPU;
            for (int j = 0; j < rq.size; ++j) {
                rq.myQueue[j]->waitingTime +=CPUprocess->remainingCPU;
            }

            CPUprocess->finishTime = time;          // assign these values
            CPUprocess->finished = true;
            CPUprocess->turnAroundTime = CPUprocess->CPUburstTime+CPUprocess->waitingTime;
            procInCPU = false;
            --noOfRemainingProc;
        }
    }
    // print to console and file
    printAndOutput(allProcess,numOfprocess,2,quantum);
}


void printAndOutput(process allProcess[],int numOfprocess,int typeOfAlgorithm, int quantum){

    string algorithmName;
    float totalCPUtime = 0;
    float totalTurnAroundTime = 0;
    float totalWaitingTime = 0;
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


    cout << string(90,'*') << endl;
    cout << string(24,'*') << " Scheduling algorithm: " << algorithmName << string(21,'*') << endl;
    if(quantum != -100){
        cout << string(24,'*') << " ( No. of Task = : "<< numOfprocess << " Quantum = " << quantum << " ) " << string(31,'*') << endl;
    }
    cout << string(90,'*') << endl << endl;

    cout << " PID " << " Arrival "<< " CPU-Burst "<<  " Finish "
         << " Waiting-Time "<< " Turnaround "<< " No.of Context-Switch \n";

    for (int i = 0; i < numOfprocess; ++i) {
        cout << fixed << setprecision(1)
             << " " << setw(6) << left << allProcess[i].PID
             << " " << setw(9) << left << allProcess[i].arrivalTime
             << " " << setw(8) << left << allProcess[i].CPUburstTime
             << " " << setw(10) << left << allProcess[i].finishTime
             << " " << setw(11) << left << allProcess[i].waitingTime
             << " " << setw(13) << left << allProcess[i].turnAroundTime
             << " " << allProcess[i].contextSwitchNumber
             << " " << endl;

        totalCPUtime += allProcess[i].CPUburstTime;
        totalWaitingTime += allProcess[i].waitingTime;
        totalTurnAroundTime += allProcess[i].turnAroundTime;
        totalContextSwitch += allProcess[i].contextSwitchNumber;
    }


    cout << fixed << setprecision(2)
         << "\nAverage CPU burst time = " << totalCPUtime/numOfprocess << " ms\n"
         << "Average waiting time = " << totalWaitingTime/numOfprocess << " ms\n"
         << "Average turn-around time = " << totalTurnAroundTime/numOfprocess << " ms\n"
         << "Total No. of Context Switching Performed = " << totalContextSwitch << endl;
}