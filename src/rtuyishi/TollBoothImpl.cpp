/*
 *  TollBoothImpl.cpp
 *
 *  Created on: April 16, 2025
 *  Author: rtuyishi
 */ 

 #include "TollBooth.h"
 #include <cmath>
 
 // Initializing PriorityQueue with null pointers and zero size
 PriorityQueue::PriorityQueue() : front(NULL), rear(NULL), size(0) {}
 
 // Deallocating PriorityQueue by removing all nodes
 PriorityQueue::~PriorityQueue() {
     while (!isEmpty()) dequeue();
 }
 
 // Adding a vehicle to the PriorityQueue, prioritizing emergency vehicles
 void PriorityQueue::enqueue(Vehicle vehicle) {
     // Creating a new node for the vehicle
     Node* newNode = new Node;
     newNode->vehicle = vehicle;
     newNode->next = NULL;
     // Checking if queue is empty and setting as first node if true
     if (isEmpty()) {
         front = rear = newNode;
     } else if (vehicle.type == EMERGENCY) {
         // Traversing to find insertion point for emergency vehicle
         Node *current = front, *prev = NULL;
         while (current && current->vehicle.type == EMERGENCY) {
             prev = current;
             current = current->next;
         }
         // Inserting emergency vehicle after other emergency vehicles
         if (prev) {
             newNode->next = prev->next;
             prev->next = newNode;
             // Updating rear if new node is at end
             if (!newNode->next) rear = newNode;
         } else {
             // Placing emergency vehicle at front
             newNode->next = front;
             front = newNode;
         }
     } else {
         // Appending regular vehicle to the rear
         rear->next = newNode;
         rear = newNode;
     }
     // Incrementing queue size
     size++;
 }
 
 // Removing and returning the front vehicle from the PriorityQueue
 Vehicle PriorityQueue::dequeue() {
     // Checking if queue is empty and returning dummy vehicle if true
     if (isEmpty()) {
         Vehicle dummy;
         dummy.vehicleId = -1;
         return dummy;
     }
     // Storing front node for deletion
     Node* temp = front;
     // Retrieving vehicle data
     Vehicle vehicle = temp->vehicle;
     // Advancing front pointer
     front = front->next;
     // Setting rear to null if queue becomes empty
     if (!front) rear = NULL;
     // Deleting temporary node
     delete temp;
     // Decrementing queue size
     size--;
     return vehicle;
 }
 
 // Checking if PriorityQueue is empty
 bool PriorityQueue::isEmpty() const { return !front; }
 
 // Returning current size of PriorityQueue
 int PriorityQueue::getSize() const { return size; }
 
 // Initializing Stack with null top pointer
 Stack::Stack() : top(NULL) {}
 
 // Deallocating Stack by removing all nodes
 Stack::~Stack() {
     while (!isEmpty()) pop();
 }
 
 // Adding a vehicle to the top of the Stack
 void Stack::push(Vehicle vehicle) {
     // Creating a new node for the vehicle
     Node* newNode = new Node;
     newNode->vehicle = vehicle;
     // Linking new node to current top
     newNode->next = top;
     // Setting new node as top
     top = newNode;
 }
 
 // Removing and returning the top vehicle from the Stack
 Vehicle Stack::pop() {
     // Checking if stack is empty and returning dummy vehicle if true
     if (isEmpty()) {
         Vehicle dummy;
         dummy.vehicleId = -1;
         return dummy;
     }
     // Storing top node for deletion
     Node* temp = top;
     // Retrieving vehicle data
     Vehicle vehicle = temp->vehicle;
     // Advancing top pointer
     top = top->next;
     // Deleting temporary node
     delete temp;
     return vehicle;
 }
 
 // Checking if Stack is empty
 bool Stack::isEmpty() const { return !top; }
 
 // Initializing BoothStack with null top pointer
 BoothStack::BoothStack() : top(NULL) {}
 
 // Deallocating BoothStack by removing all nodes
 BoothStack::~BoothStack() {
     while (!isEmpty()) pop();
 }
 
 // Adding a booth index to the top of the BoothStack
 void BoothStack::push(int boothIndex) {
     // Creating a new node for the booth index
     IntNode* newNode = new IntNode;
     newNode->boothIndex = boothIndex;
     // Linking new node to current top
     newNode->next = top;
     // Setting new node as top
     top = newNode;
 }
 
 // Removing and returning the top booth index from the BoothStack
 int BoothStack::pop() {
     // Checking if stack is empty and returning -1 if true
     if (isEmpty()) return -1;
     // Storing top node for deletion
     IntNode* temp = top;
     // Retrieving booth index
     int boothIndex = temp->boothIndex;
     // Advancing top pointer
     top = top->next;
     // Deleting temporary node
     delete temp;
     return boothIndex;
 }
 
 // Checking if BoothStack is empty
 bool BoothStack::isEmpty() const { return !top; }
 
 // Initializing tollbooth with given ID and default stats
 tollbooth::tollbooth(int boothId) : id(boothId), isActive(false), availableTime(0) {
     // Setting initial booth statistics to zero
     stats.totalVehicles = 0;
     stats.emergencyVehicles = 0;
     stats.regularVehicles = 0;
     stats.cashPayments = 0;
     stats.mobilePayments = 0;
     stats.cardPayments = 0;
 }
 
 // Checking if tollbooth is available to process a vehicle
 bool tollbooth::isAvailable(int currentTime) const {
     return isActive && vehicleStack.isEmpty() && currentTime >= availableTime;
 }
 
 // Processing a vehicle by adding it to the booth's stack and updating stats
 void tollbooth::processVehicle(Vehicle vehicle, int currentTime) {
     // Pushing vehicle onto the booth's stack
     vehicleStack.push(vehicle);
     // Updating booth's available time based on processing time
     availableTime = currentTime + vehicle.processingTime;
     // Incrementing total vehicle count
     stats.totalVehicles++;
     // Updating emergency or regular vehicle count
     vehicle.type == EMERGENCY ? stats.emergencyVehicles++ : stats.regularVehicles++;
     // Incrementing payment method count
     switch (vehicle.paymentMethod) {
         case CASH: stats.cashPayments++; break;
         case MOBILE: stats.mobilePayments++; break;
         case CARD: stats.cardPayments++; break;
     }
 }
 
 // Returning booth statistics
 BoothStats tollbooth::getStats() const { return stats; }
 
 // Initializing TollboothStation with default parameters and vehicle array
 TollboothStation::TollboothStation() : lambda(0), initialBooths(0), openThreshold(0), closeThreshold(0),
     emergencyProb(0), cashProb(0), mobileProb(0), simulationTime(0), maxBooths(0), minQueueSize(0),
     activeBooths(0), maxActiveBooths(0), maxQueueLength(0), totalWaitingTime(0), totalProcessingTime(0),
     totalDwellingTime(0), totalVehicles(0), vehicleCount(0), vehicleCapacity(1000), tollbooths(NULL) {
     // Allocating initial vehicle array
     vehicles = new Vehicle[vehicleCapacity];
 }
 
 // Deallocating TollboothStation resources
 TollboothStation::~TollboothStation() {
     // Deleting vehicle array
     delete[] vehicles;
     // Checking if tollbooths exist
     if (tollbooths) {
         // Deleting each tollbooth
         for (int i = 0; i < maxBooths; i++) delete tollbooths[i];
         // Deleting tollbooth array
         delete[] tollbooths;
     }
 }
 
 // Generating vehicles based on Poisson process
 void TollboothStation::generateVehicles() {
     // Resetting vehicle count
     vehicleCount = 0;
     // Initializing current time
     double currentTime = 0.0;
     // Generating vehicles until simulation time is exceeded
     while (currentTime < simulationTime) {
         // Calculating next arrival time using exponential distribution
         currentTime += -std::log((double)rand() / (RAND_MAX + 1.0)) / lambda;
         // Breaking if simulation time is exceeded
         if (currentTime > simulationTime) break;
 
         // Checking if vehicle array needs resizing
         if (vehicleCount >= vehicleCapacity) {
             // Doubling vehicle capacity
             vehicleCapacity *= 2;
             // Allocating new vehicle array
             Vehicle* newList = new Vehicle[vehicleCapacity];
             // Copying existing vehicles to new array
             for (int i = 0; i < vehicleCount; i++) newList[i] = vehicles[i];
             // Deleting old array
             delete[] vehicles;
             // Setting new array
             vehicles = newList;
         }
 
         // Creating new vehicle
         Vehicle vehicle;
         // Assigning unique vehicle ID
         vehicle.vehicleId = vehicleCount + 1;
         // Setting arrival time
         vehicle.arrivalTime = currentTime;
         // Determining vehicle type based on emergency probability
         vehicle.type = (rand() / (double)RAND_MAX < emergencyProb) ? EMERGENCY : REGULAR;
         // Assigning payment method based on probabilities
         vehicle.paymentMethod = (rand() / (double)RAND_MAX < cashProb) ? CASH :
                                (rand() / (double)RAND_MAX < mobileProb) ? MOBILE : CARD;
         // Initializing time and booth fields
         vehicle.waitingTime = vehicle.dwellingTime = vehicle.boothNumber = 0;
         // Calculating processing time
         vehicle.processingTime = determineProcessingTime(vehicle.type, vehicle.paymentMethod);
         // Adding vehicle to array
         vehicles[vehicleCount++] = vehicle;
     }
     // Setting total vehicle count
     totalVehicles = vehicleCount;
 }
 
 // Determining processing time based on vehicle type and payment method
 int TollboothStation::determineProcessingTime(VehicleType type, PaymentMethod method) {
     // Assigning fixed time for emergency vehicles
     if (type == EMERGENCY) return 2;
     // Assigning random time based on payment method
     switch (method) {
         case CASH: return 7 + rand() % 4;
         case MOBILE: return 5 + rand() % 2;
         case CARD: return 2 + rand() % 3;
         default: return 5;
     }
 }
 
 // Opening a new booth if conditions are met
 void TollboothStation::openBooth() {
     // Checking if more booths can be opened and stack is not empty
     if (activeBooths < maxBooths && !boothStack.isEmpty()) {
         // Retrieving booth index from stack
         int boothIndex = boothStack.pop();
         // Activating booth
         tollbooths[boothIndex]->isActive = true;
         // Incrementing active booth count
         activeBooths++;
         // Updating maximum active booths if necessary
         if (activeBooths > maxActiveBooths) maxActiveBooths = activeBooths;
     }
 }
 
 // Closing a booth if conditions are met
 void TollboothStation::closeBooth() {
     // Checking if more than one booth is active
     if (activeBooths > 1) {
         // Selecting highest-indexed active booth
         int boothIndex = activeBooths - 1;
         // Deactivating booth
         tollbooths[boothIndex]->isActive = false;
         // Pushing booth index to stack
         boothStack.push(boothIndex);
         // Decrementing active booth count
         activeBooths--;
     }
 }
 
 // Managing booth operations at current time
 void TollboothStation::handleBooths(int currentTime) {
     // Retrieving current queue size
     int queueSize = vehicleQueue.getSize();
     // Updating maximum queue length if necessary
     if (queueSize > maxQueueLength) maxQueueLength = queueSize;
 
     // Opening booth if queue is large enough
     if (queueSize >= minQueueSize && queueSize > openThreshold && activeBooths < maxBooths) openBooth();
     // Closing booth if queue is small enough
     if (queueSize < closeThreshold && activeBooths > 1) closeBooth();
 
     // Processing each booth
     for (int i = 0; i < maxBooths; i++) {
         // Skipping inactive booths
         if (!tollbooths[i]->isActive) continue;
         // Completing vehicle processing if available
         if (!tollbooths[i]->vehicleStack.isEmpty() && currentTime >= tollbooths[i]->availableTime) {
             // Removing finished vehicle
             Vehicle finished = tollbooths[i]->vehicleStack.pop();
             // Updating vehicle record with dwelling time and booth number
             for (int j = 0; j < vehicleCount; j++) {
                 if (vehicles[j].vehicleId == finished.vehicleId) {
                     vehicles[j].dwellingTime = vehicles[j].waitingTime + vehicles[j].processingTime;
                     vehicles[j].boothNumber = i + 1;
                     break;
                 }
             }
         }
 
         // Assigning new vehicle to booth if available
         if (tollbooths[i]->isAvailable(currentTime) && !vehicleQueue.isEmpty()) {
             // Dequeuing next vehicle
             Vehicle nextVehicle = vehicleQueue.dequeue();
             // Calculating waiting time
             nextVehicle.waitingTime = currentTime - (int)nextVehicle.arrivalTime;
             // Assigning booth number
             nextVehicle.boothNumber = i + 1;
             // Updating vehicle record
             for (int j = 0; j < vehicleCount; j++) {
                 if (vehicles[j].vehicleId == nextVehicle.vehicleId) {
                     vehicles[j].waitingTime = nextVehicle.waitingTime;
                     vehicles[j].boothNumber = nextVehicle.boothNumber;
                     break;
                 }
             }
             // Processing vehicle at booth
             tollbooths[i]->processVehicle(nextVehicle, currentTime);
         }
     }
 }
 
 // Running the tollbooth simulation
 void TollboothStation::simulate() {
     // Clearing existing queue
     while (!vehicleQueue.isEmpty()) vehicleQueue.dequeue();
     // Generating vehicles for simulation
     generateVehicles();
 
     // Initializing vehicle index
     int currentVehicleIndex = 0;
     // Simulating each time unit
     for (int currentTime = 0; currentTime <= simulationTime; currentTime++) {
         // Enqueuing vehicles arriving at current time
         while (currentVehicleIndex < vehicleCount && (int)vehicles[currentVehicleIndex].arrivalTime == currentTime) {
             vehicleQueue.enqueue(vehicles[currentVehicleIndex++]);
         }
         // Handling booth operations
         handleBooths(currentTime);
     }
 
     // Processing remaining queued vehicles
     int extraTime = simulationTime;
     while (!vehicleQueue.isEmpty()) handleBooths(++extraTime);
 
     // Checking for vehicles still in booths
     bool stillProcessing = false;
     for (int i = 0; i < maxBooths; i++) {
         if (!tollbooths[i]->vehicleStack.isEmpty()) stillProcessing = true;
     }
 
     // Handling remaining booth processing
     if (stillProcessing) {
         // Setting maximum extra processing time
         int maxExtraProcessingTime = 100, extraProcessingTimeCount = 0;
         // Continuing until no vehicles remain or limit reached
         while (stillProcessing && extraProcessingTimeCount < maxExtraProcessingTime) {
             // Handling booths at extra time
             handleBooths(++extraTime);
             // Incrementing extra time count
             extraProcessingTimeCount++;
             // Checking for remaining vehicles
             stillProcessing = false;
             for (int i = 0; i < maxBooths; i++) {
                 if (!tollbooths[i]->vehicleStack.isEmpty()) {
                     stillProcessing = true;
                     break;
                 }
             }
         }
 
         // Handling stuck vehicles if limit reached
         if (extraProcessingTimeCount >= maxExtraProcessingTime) {
             for (int i = 0; i < maxBooths; i++) {
                 if (!tollbooths[i]->vehicleStack.isEmpty()) {
                     // Removing stuck vehicle
                     Vehicle stuckVehicle = tollbooths[i]->vehicleStack.pop();
                     // Updating vehicle record
                     for (int j = 0; j < vehicleCount; j++) {
                         if (vehicles[j].vehicleId == stuckVehicle.vehicleId) {
                             vehicles[j].dwellingTime = vehicles[j].waitingTime + vehicles[j].processingTime;
                             vehicles[j].boothNumber = i + 1;
                             break;
                         }
                     }
                 }
             }
         }
     }
 
     // Calculating total times for processed vehicles
     totalWaitingTime = totalProcessingTime = totalDwellingTime = 0;
     int processedVehicles = 0;
     for (int i = 0; i < vehicleCount; i++) {
         if (vehicles[i].dwellingTime > 0) {
             // Summing waiting, processing, and dwelling times
             totalWaitingTime += vehicles[i].waitingTime;
             totalProcessingTime += vehicles[i].processingTime;
             totalDwellingTime += vehicles[i].dwellingTime;
             // Incrementing processed vehicle count
             processedVehicles++;
         }
     }
 }
 
 // Outputting simulation results to file
 void TollboothStation::outputs(std::ofstream& outFile, int testCase) {
     // Writing test case header
     outFile << "Test Case " << testCase << ":\n"
             << "Vehicle ID | Arrival Time | Waiting Time | Processing Time | Dwelling Time | Vehicle Type | Payment Method | Booth\n"
             << "---------------------------------------------------------------------------------------------------------------\n";
 
     // Initializing processed vehicle count
     int processedVehicles = 0;
     // Writing data for each processed vehicle
     for (int i = 0; i < vehicleCount; i++) {
         if (vehicles[i].dwellingTime > 0) {
             // Incrementing processed vehicle count
             processedVehicles++;
             // Formatting and outputting vehicle details
             outFile << std::setw(10) << vehicles[i].vehicleId << " | "
                     << std::setw(12) << std::fixed << std::setprecision(2) << vehicles[i].arrivalTime << " | "
                     << std::setw(12) << vehicles[i].waitingTime << " | "
                     << std::setw(15) << vehicles[i].processingTime << " | "
                     << std::setw(13) << vehicles[i].dwellingTime << " | "
                     << std::setw(12) << (vehicles[i].type == EMERGENCY ? "Emergency" : "Regular") << " | "
                     << std::setw(13) << (vehicles[i].paymentMethod == CASH ? "Cash" :
                                         vehicles[i].paymentMethod == MOBILE ? "Mobile" : "Card") << " | "
                     << vehicles[i].boothNumber << "\n";
         }
     }
 
     // Adding newline for readability
     outFile << "\n";
     // Writing booth statistics
     for (int i = 0; i < maxBooths; i++) {
         // Retrieving booth stats
         BoothStats stats = tollbooths[i]->getStats();
         // Outputting stats for booths with processed vehicles
         if (stats.totalVehicles > 0) {
             outFile << "Booth " << i + 1 << ": Processed " << stats.totalVehicles 
                     << " vehicles (Emergency: " << stats.emergencyVehicles << ", Regular: " << stats.regularVehicles 
                     << ", Cash: " << stats.cashPayments << ", Mobile: " << stats.mobilePayments 
                     << ", Card: " << stats.cardPayments << ")\n";
         }
     }
 
     // Writing maximum queue length
     outFile << "\nMaximum Queue Length: " << maxQueueLength << "\n";
     // Calculating and writing average times
     if (processedVehicles > 0) {
         outFile << std::fixed << std::setprecision(1)
                 << "Average Waiting Time: " << (double)totalWaitingTime / processedVehicles << " seconds\n"
                 << "Average Processing Time: " << (double)totalProcessingTime / processedVehicles << " seconds\n"
                 << "Average Dwelling Time: " << (double)totalDwellingTime / processedVehicles << " seconds\n";
     } else {
         // Outputting zero averages if no vehicles processed
         outFile << "Average Waiting Time: 0.0 seconds\n"
                 << "Average Processing Time: 0.0 seconds\n"
                 << "Average Dwelling Time: 0.0 seconds\n";
     }
     // Writing maximum booths open
     outFile << "Maximum Booths Open: " << maxActiveBooths << "\n\n";
 }
 
 // Simulating vehicle arrivals and tollbooth operations
 void simulateArrival(double lambda, int initialBooths, int openThreshold, int closeThreshold,
                     double emergencyProb, double cashProb, double mobileProb, int simulationTime,
                     int maxBooths, int minQueueSize, int testCase, std::ofstream& outFile) {
     // Checking if lambda is invalid
     if (lambda <= 0) {
         // Outputting message for no vehicle generation
         outFile << "Test Case " << testCase << ":\nNo vehicles generated (lambda <= 0).\n\n";
         return;
     }
 
     // Creating tollbooth station
     TollboothStation station;
     // Setting simulation parameters
     station.lambda = lambda;
     station.initialBooths = initialBooths;
     station.openThreshold = openThreshold;
     station.closeThreshold = closeThreshold;
     station.emergencyProb = emergencyProb;
     station.cashProb = cashProb;
     station.mobileProb = mobileProb;
     station.simulationTime = simulationTime;
     station.maxBooths = maxBooths;
     station.minQueueSize = minQueueSize;
 
     // Allocating tollbooth array
     station.tollbooths = new tollbooth*[maxBooths];
     // Initializing tollbooths and pushing indices to stack
     for (int i = 0; i < maxBooths; i++) {
         station.tollbooths[i] = new tollbooth(i + 1);
         station.boothStack.push(i);
     }
 
     // Setting initial active booths
     station.activeBooths = std::min(std::max(initialBooths, 1), maxBooths);
     // Activating initial booths
     for (int i = 0; i < station.activeBooths; i++) {
         // Popping booth index from stack
         int boothIndex = station.boothStack.pop();
         // Activating booth
         station.tollbooths[boothIndex]->isActive = true;
     }
     // Setting initial maximum active booths
     station.maxActiveBooths = station.activeBooths;
 
     // Running simulation
     station.simulate();
     // Outputting results
     station.outputs(outFile, testCase);
 }