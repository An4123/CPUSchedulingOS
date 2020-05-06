// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//
// Comments:

#include "readyQueue.h"
#include <iostream>




readyQueue::readyQueue() {
    for (auto &i : myQueue) {
        i = nullptr;
    }
    size = 0;
}

void readyQueue::push(process * p) {
    ++size;
    if(size > MAX_PROCESS){
        std::cout<< "maximum allowed procedure exceeded\n";
    }
    myQueue[size-1]= p;
}

process* readyQueue::pop() {
    process * temp;
    temp = myQueue[0];
    --size;
    for (int i = 0; i <size ; ++i) {
        myQueue[i]=myQueue[i+1];
    }
    myQueue[size]= nullptr;
    return temp;
}

void readyQueue::sortByTime() {

    process* temp;
    bool swapped = true;
    int idx = 0;

    while (swapped) {
        swapped = false;
        ++idx;
        for (int i = 0; i < size - idx; ++i) {
            if (myQueue[i]->remainingCPU > myQueue[i + 1]->remainingCPU) {
                temp = myQueue[i];
                myQueue[i] = myQueue[i + 1];
                myQueue[i + 1] = temp;
                swapped = true;
            }
        }
    }
}

void readyQueue::sortByPriority() {

    process * temp;
    bool swapped = true;
    int idx = 0;

    while (swapped) {
        swapped = false;
        ++idx;
        for (int i = 0; i < size - idx; ++i) {
            if (myQueue[i]->getPriority() > myQueue[i + 1]->getPriority()) {
                temp = myQueue[i];
                myQueue[i] = myQueue[i + 1];
                myQueue[i + 1] = temp;
                swapped = true;
            }
        }
    }
}

bool readyQueue::procExist(process& p) {
    bool result = false;
    for (int i = 0; i < size; ++i) {
        if(&p == myQueue[i]){
            result = true;
            break;
        }
    }
    return result;
}
