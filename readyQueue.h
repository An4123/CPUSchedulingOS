// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//
// Comments:

#ifndef CPUSCHEDULINGOS_READYQUEUE_H
#define CPUSCHEDULINGOS_READYQUEUE_H

#include "process.h"

const int MAX_PROCESS = 101;

class readyQueue {
public:
    process* myQueue[MAX_PROCESS];
    int size;
    readyQueue();
    void push(process*);
    process* pop();

    void sortByTime();
    void sortByPriority();
    bool procExist(process&);

};


#endif //CPUSCHEDULINGOS_READYQUEUE_H
