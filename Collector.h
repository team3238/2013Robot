#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <WPILib.h>
#include "Portnums.h"

enum {
	limbo,
	isEmpty,
	loaded,
	running,
};

enum {
	stepCloseFloor,
	stepOpenFloor, 
	stepCloseServoLock,
	stepOpenServoLock,
	stepWait,
	stepModeEmpty,
	stepModeLoaded,
};

//Timings & lock values
const double ServoLockTime = 0.5;
const double floorTime = 0.1;
const float unlockRight = 0;
const float unlockLeft = 1;
const float lockRight = 1;
const float lockLeft = 0;

class QueueItem{
public:
	
	QueueItem(int data);
	~QueueItem();
	void AddItem(QueueItem* list);
	int mData;
	QueueItem* next;
};


class Queue {
public:
	
	Queue();
	void push(int data);
	void pop();
	bool empty();
	void dump();
	int top();
	QueueItem* list;
};


class Collector : public Queue {
public:
    // These methods are visable (callable) to anyone using this class
    // If you wish to add or remove any public methods, talk to Nick Papadakis

    // Constructor
    Collector(UINT32 BotFloorOpenSwitch, UINT32 BotFloorCloseSwitch, UINT32 bucketThingy);
    
    //Initializes
    void Init();
    
    //Starts the collector
    void start();
    
    //Checks for a frisbee
    bool isFrisbeeReady();
    
    //Checks to see if we're done dropping a disc
    bool doneDropping();
    
    //Drops a disc
    void dropDisc();
    
    //Changes the floor control to manual mode
    void manualMode(bool mode);
    
    //Manual control of the collector
    void manualFloorControl(int direction);
    
    //Various test functions for debugging
    void testOpenServoLock();
    void testCloseServoLock();
    void testOpenFloor();
    void testCloseFloor();
    bool testFloorClosed();
    bool testFloorOpened();
    bool testHaveFrisbee();
    string getState();

    //General processing
    void Idle();
    
    //Shuts off motors, empties Queue
    void Disable();

private:
    // Put useful functions and variables here
	void openFloor();
	void openServoLock();
	void closeServoLock();
	void shutoffFloor();
	void closeFloor();
	void checkStep();
	void startStep();
	bool isFloorClose();
	bool isFloorOpen();
	bool Initialized;
	bool manual;
	DigitalInput *bucketStatusSwitch; 
	DigitalInput *BottomFloorOpenSwitch;
	DigitalInput *BottomFloorCloseSwitch;
	Relay *FloorDrive;
	Servo *ServoLockRight;
	Servo *ServoLockLeft;
	Timer *ServoLockTimer;
	Timer *floorTimer;
	int state;
};

#endif
