//
// Created by super on 5/6/2020.
//

#ifndef CPUSCHEDULINGOS_PROCESS_H
#define CPUSCHEDULINGOS_PROCESS_H


class process {
        private:
// creating process with after reading file
        int PID;
        int arrivalTime;
        int CPUburstTime;
        int priority;

        public:
//constructors
        process();

// when we read directly from file
        explicit process(int[]);

//setter & getter

        int getPID();
        int getArrivalTime();
        int getCPUburstTime();
        int getPriority();

//public variable

        int remainingCPU;
        int contextSwitchNumber;
// bool waiting;
        bool finished;
// bool prempted;
        bool firstTimeInCPU; // useful to calculate responsetime

// variable to track timing

        float responseTime; // how long it has to wait go get into cpu
        float finishTime; // what time did it finish
        float waitingTime; // time to wait outside of cpu after being ready
        float turnAroundTime; // total time to execute. waiting + execution.
};


#endif //CPUSCHEDULINGOS_PROCESS_H
