/*
 *  TollBooth.h
 *
 *  Created on: April 16, 2025
 *  Author: rtuyishi
 */

#ifndef TOLLBOOTH_H
#define TOLLBOOTH_H


#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

// Defining vehicle types (Emergency or Regular)
enum VehicleType { EMERGENCY, REGULAR };

// Defining payment methods (Cash, Mobile, or Card)
enum PaymentMethod { CASH, MOBILE, CARD };

// Storing vehicle information and tracking its journey through the toll system
struct Vehicle {
    int vehicleId;
    double arrivalTime;
    int waitingTime, processingTime, dwellingTime, boothNumber;
    VehicleType type;
    PaymentMethod paymentMethod;
};

// Tracking statistical data for booth operations
struct BoothStats {
    int totalVehicles, emergencyVehicles, regularVehicles;
    int cashPayments, mobilePayments, cardPayments;
};

// Creating nodes for linked list implementation
struct Node {
    Vehicle vehicle;
    Node* next;
};

// Creating nodes for booth index tracking
struct IntNode {
    int boothIndex;
    IntNode* next;
};

// Managing vehicle queue with priority handling
struct PriorityQueue {
    Node *front, *rear;
    int size;

    PriorityQueue();
    ~PriorityQueue();
    void enqueue(Vehicle vehicle);
    Vehicle dequeue();
    bool isEmpty() const;
    int getSize() const;
};

// Managing vehicle stack for last-in-first-out operations
struct Stack {
    Node* top;

    Stack();
    ~Stack();
    void push(Vehicle vehicle);
    Vehicle pop();
    bool isEmpty() const;
};

// Managing booth indices using stack implementation
struct BoothStack {
    IntNode* top;

    BoothStack();
    ~BoothStack();
    void push(int boothIndex);
    int pop();
    bool isEmpty() const;
};

// Managing individual toll booth operations and statistics
struct tollbooth {
    int id;
    bool isActive;
    Stack vehicleStack;
    int availableTime;
    BoothStats stats;

    tollbooth(int boothId);
    bool isAvailable(int currentTime) const;
    void processVehicle(Vehicle vehicle, int currentTime);
    BoothStats getStats() const;
};

// Managing overall tollbooth station operations and simulation
struct TollboothStation {
    double lambda, emergencyProb, cashProb, mobileProb;
    int initialBooths, openThreshold, closeThreshold, simulationTime, maxBooths, minQueueSize;
    PriorityQueue vehicleQueue;
    tollbooth** tollbooths;
    BoothStack boothStack;
    int activeBooths, maxActiveBooths, maxQueueLength;
    int totalWaitingTime, totalProcessingTime, totalDwellingTime, totalVehicles;
    Vehicle* vehicles;
    int vehicleCount, vehicleCapacity;

    TollboothStation();
    ~TollboothStation();
    void simulate();
    void handleBooths(int currentTime);
    void generateVehicles();
    int determineProcessingTime(VehicleType type, PaymentMethod method);
    void openBooth();
    void closeBooth();
    void outputs(std::ofstream& outFile, int testCase);
};

// Simulating vehicle arrivals and processing at the tollbooth station
void simulateArrival(double lambda, int initialBooths, int openThreshold, int closeThreshold,
                    double emergencyProb, double cashProb, double mobileProb, int simulationTime,
                    int maxBooths, int minQueueSize, int testCase, std::ofstream& outFile);

#endif // TOLLBOOTH_H