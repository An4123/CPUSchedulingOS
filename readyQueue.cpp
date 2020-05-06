// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//

#include <iostream>
#include "readyQueue.h"

readyQueue::readyQueue() {          // populate the queue will nullptr

    for (auto &i : myQueue) {
        i = nullptr;
    }
    size = 0;                       // set size to zero
}

void readyQueue::push(process * p) {                // add process to queue
    ++size;                                         // add one to size
    if(size > MAX_PROCESS){                         // check if max is reached
        std::cout<< "maximum allowed procedure exceeded\n";
    }
    myQueue[size-1]= p;                             // add process
}


process* readyQueue::pop() {                        // remove process from queue
    process * temp;
    temp = myQueue[0];
    --size;                                         // sub one from size
    for (int i = 0; i <size ; ++i) {
        myQueue[i]=myQueue[i+1];
    }
    myQueue[size]= nullptr;                         // set last to nullptr
    return temp;
}
void readyQueue::sortByTime() {                     // sort the algorithm by the time it took
    process* temp;
    bool swapped = true;
    int idx = 0;
    while (swapped) {                               // while still swapping
        swapped = false;
        ++idx;
        for (int i = 0; i < size - idx; ++i) {      // sort the queue by remainingCPU
            if (myQueue[i]->remainingCPU > myQueue[i + 1]->remainingCPU) {
                temp = myQueue[i];
                myQueue[i] = myQueue[i + 1];
                myQueue[i + 1] = temp;
                swapped = true;
            }
        }
    }
}


bool readyQueue::procExist(process& p) {      // check if any other process are in queue
    bool result = false;
    for (int i = 0; i < size; ++i) {          // checks each element
        if(&p == myQueue[i]){                 // if there is process
            result = true;                    // will return true
            break;
        }
    }
    return result;
}