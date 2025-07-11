/*
 *  main.cpp
 *
 *  Created on: April 16, 2025
 *  Author: rtuyishi
 */
#include "TollBooth.h"
#include <iostream>
#include <fstream>

int main() {
    // Setting random seed using current time
    std::srand(static_cast<unsigned>(std::time(NULL)));

    // Opening input file for reading test cases
    std::ifstream inFile("data/input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file data/input.txt\n";
        return 1;
    }

    // Opening output file for writing simulation results
    std::ofstream outFile("data/output.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file data/output.txt\n";
        return 1;
    }

    // Reading number of test cases
    int numTestCases;
    inFile >> numTestCases;

    // Processing each test case
    for (int testCase = 1; testCase <= numTestCases; ++testCase) {
        // Declaring variables for simulation parameters
        double lambda, emergencyProb, cashProb, mobileProb;
        int initialBooths, openThreshold, closeThreshold, simulationTime, maxBooths, minQueueSize;

        // Reading simulation parameters from input file
        inFile >> lambda >> initialBooths >> openThreshold >> closeThreshold 
               >> emergencyProb >> cashProb >> mobileProb >> simulationTime 
               >> maxBooths >> minQueueSize;

        // Running simulation with given parameters
        simulateArrival(lambda, initialBooths, openThreshold, closeThreshold,
                        emergencyProb, cashProb, mobileProb, simulationTime,
                        maxBooths, minQueueSize, testCase, outFile);
    }

    // Closing input and output files
    inFile.close();
    outFile.close();
    std::cout << "Simulation completed. Results in data/output.txt\n";

    return 0;
}