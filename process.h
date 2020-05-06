// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3
//

#ifndef CPUSCHEDULINGOS_PROCESS_H
#define CPUSCHEDULINGOS_PROCESS_H
class process {                  // class for each process
public:
    int PID;
    int arrivalTime;
    int CPUburstTime;

    process();                   //constructors
    explicit process(int[]);


    int remainingCPU;
    int contextSwitchNumber;
    bool finished;


    float finishTime;                 // keep track of finished time
    float waitingTime;                // time waiting for cpu to be ready
    float turnAroundTime;             // waiting + execution.

};
#endif //CPUSCHEDULINGOS_PROCESS_H