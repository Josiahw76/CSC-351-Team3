#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Template created by Josiah

// Yes, this is the data structures PQ.
// This PQ was written by Josiah and Dylan.
#include "pidPQ.h"

/*
The program instructions are:
S value:  Set the program’s value to the provide value.  E.g., S 1122.
A value: Add the provided value to the program’s value.  E.g., A 24
D value: Deduct the provided value from the program’s value. E.g., D 37

F offset: Create a new process that is an exact copy of the current process and:
o	setting the ppid (parent process identifier) to the current process identifier (pid).
o	Setting the new process’s PC to the PC of the current process’s PC plus one.
o	Setting the current process PC to be “offset” positions beyond the next instruction of the current process.
o	Set the new process’s values accordingly:
•	Remember, all processes start with a priority of 0.
•	Time used is set to 0
•	Start time is set to the current system clock value.
•	Block time is set 0.
•	Value is set to the value of the parent process’s value.
o	Enqueue the new process to the ReadyState queue.

B value: Block the current process from running by the “value” number of clock ticks.  Upon unblocking, the process’s priority will be given the next higher priority than when it was blocked.

R fname: Replace the current process’s program text with the text found in the file named “fname” and set the current process’s PC to 0.

E:  End the program, deleting the process altogether.
*/




// Defined according to the assignment
enum PRIORITY {
	HIGH = 0,
	MED = 1,
	LOW = 2,
	MIN = 3
};

// Clunky name for the CPU object in the scheduler
struct activeProcess {
	unsigned int runningPID; 	// PID of active process
	unsigned int timeLeft; 		// timeslice of active process
}

// Referred to as PCB entry in the assignment
struct PCB {
	unsigned int PC;	// program counter
	unsigned int PID;	// process identifier
	unsigned int PPID;	// parent process identifier
	PRIORITY priority;	// priority of the process
	unsigned int timeslice;	// # of clock ticks before the process is
				// put back on the ReadyState queue
				// Timeslice is set by the following priorities:
				// 0	:	1
				// 1	:	2
				// 2	:	4
				// 3	:	8
	unsigned int startTime;	// Set to the sysClock value on initialization
	unsigned int timeUsed;	// For each clock tick used, this is incremented
	unsigned int blockTime;	// Amount of clock ticks that process will be
				// blocked. If in blocked state, decrement
				// by one each time sysClock is updated.
				// The execution of this statement does not
				// count for blocking period(?)
	char *instruction; 	// This can just be the whole string until '\n'
	unsigned int instructionCount; // ?
};


class Scheduler {
	private:
		unsigned int sysClock;
		unsigned int PCBcapacity;
		unsigned int PCBcount;
		unsigned int processCount;
		const unsigned int priorityCount = 4;
		activeProcess CPU;				
		PCB *PCBtable;	
		PQ *ReadyState;
		int programValue;

		void step();	// If there is a running process, 
				// execute 
				// the current instruction of the running 
				// process. Update the System Clock, and 
				// the running process’s Time Used, 
				// time slice, and PC values accordingly. 
				// If the B instruction is executed, 
				// set the process’s block time accordingly 
				// and reduce the process’s priority.

		// Completed by Josiah
		void schedule();	
				// If there is not a running process, then 
				// dequeue one from the ReadyState queue.
				//  If no process was ready to run, then do 
				// nothing. Otherwise, if the running process
				// is blocked, swap in a new process from the 
				// ReadyState queue, if one exists.  Otherwise,
				//  if the current running process has a time 
				//  slice of 0, swap it out, decreasing it’s 
				//  priority and put it into the ReadyState 
				//  queue. and swap in a process from the 
				//  Ready State queue, if one exists.

		printState();	// This causes the complete state of all PCBs 
				// to be printed.  See the example output for 
				// the expected format. Note, for PIDs that 
				// don’t have programs, it prints nothing.

		// Completed by Josiah
		bool searchByPID(unsigned int PID, PCB * &pcb);
				// Get the pointer to the process info by
				// PID

		// Completed by Simeon
		void swapIn();	// This retrieves the highest priority process 
				// from the ReadyState queue.  If such a 
				// process is dequeued, if the process’s time 
				// slice is 0, this sets the time slice 
				// according to the priority of the process. 
				// This then updates the CPU’s PID and time 
				// slice values.

		// Completed by Simeon
		void swapOut();	// This copies the CPU’s time slice value to 
				// the process’s time slice value and then 
				// enqueues the process to the ReadyState queue.

		// Completed by Josiah
		void processBlockedStates();
				// this updates the block time for every 
				// process in the blocked state.  If the 
				// block time of a process becomes 0, it 
				// enqueues the process to the ReadyState queue.

		runSystem();	// While there are allocated PCBs, this does 
				// the following, in order; step, schedule, 
				// process blocked states, and printState.



	public:
		scheduler();	// Creates the scheduler object, does not
				// start running anything yet.

		kickOff();	// To get things going, the first process is 
				// created with the program found in a file 
				// by the name of “init.txt” and then 
				// runSystem is called.
				//
				// True access point of the scheduler.

};

#endif
