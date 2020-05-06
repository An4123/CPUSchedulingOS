// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//

#ifndef CPUSCHEDULINGOS_READYQUEUE_H
#define CPUSCHEDULINGOS_READYQUEUE_H

#include "process.h"

const int MAX_PROCESS = 101;            // MAXIMUM NUMBER OF PROCESSES

class readyQueue {                      // Class for Queue of processes
public:
    process* myQueue[MAX_PROCESS]{};    // array of processes
    int size;                           // size of queue
    readyQueue();                       // constructor
    void push(process*);                // add process
    process* pop();                     // remove process
    bool procExist(process&);           // check if process exist

    void sortByTime();                  // for the shortest algorithm

};


#endif //CPUSCHEDULINGOS_READYQUEUE_H