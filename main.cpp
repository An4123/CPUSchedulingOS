// Name: An Vo
// Date: 5/4/2020.
// Class: Operating Systems
// Assignment: Lab 3

#include <iostream>
#include <string>
#include "Functions.h"
using namespace std;

int main(int argc, char* argv[]) {
    string inputFile;
    string typeOfAlgorithm;
    int algFunc = -1;
    size_t sz;
    int quantumTime = 0;

//    long int quantumTime = 4;              // test quantumtime
//    inputFile = "input4";             // test input
//    typeOfAlgorithm = "RR";         // test algorithms

    inputFile = string(argv[1]);                    // get input file from command line
    typeOfAlgorithm = string(argv[2]);              // get algorithm from command line

    int numberOfProcess = findNumProc(inputFile);       // finding number of process
    process allProcess[numberOfProcess];        // creating the array of all process need to processed.


// assigning algorithm to a number for switch statement
    if (typeOfAlgorithm == "FCFS")
        algFunc = 0;
    if (typeOfAlgorithm == "SRTF")
        algFunc = 1;
    if (typeOfAlgorithm == "RR") {            // if RR we set number for switch and quantum time from user
        algFunc = 2;
        if (argv[3]) {
            quantumTime = stoi(argv[3], &sz);
//            quantumTime = 4;                // test quantum time
        } else {                              // if none is given
            cout << "quantum time is needed to run the round robin algorithm\n";
            quantumTime = -100;
        }
    }

    if (quantumTime == -100) {               // Return if quantum time is not given
        return 0;
    } else { // call functions accordingly
        initilizeAllProcess(inputFile, allProcess);
        switch (algFunc) {
            case 0:
                FCFS(allProcess, numberOfProcess);
                break;
            case 1:
                SRTF(allProcess, numberOfProcess);
                break;
            case 2:
                //cout << " QT" << quantumTime << endl;
                RR(allProcess, numberOfProcess, int(quantumTime));
                break;
            default:
                cout << "Error in initializing code \n";
        }

    }
    return 0;
}

