#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Template created by Josiah

// Yes, this is the data structures PQ.
// This PQ was written by Josiah and Dylan.
#include "PQ.h"

// Defined according to the assignment
enum PRIORITY {
	HIGH = 0,
	MED = 1,
	LOW = 2,
	MIN = 3
};

// Referred to as PCB entry in the assignment
struct PCB {
	unsigned int PC;	// program counter
	unsigned int PID;	// process identifier
	unsigned int PPID;	// parent process identifier
	PRIORITY priority;	// priority of the process
	unsigned int timeslice;	// Number of ticks left of runtime
				// Timeslice is set by the following priorities:
				// 0	:	8
				// 1	:	4
				// 2	:	2
				// 3	:	1
				// The table was given with the right column
				// flipped so lower priority processes were
				// given more time, but that makes
				// no sense so we're doing it this way.
	unsigned int startTime;	// Set to the sysClock value on initialization
	unsigned int timeUsed;	// For each clock tick used, this is incremented
	unsigned int blockTime;	// Amount of clock ticks that process will be
				// blocked. If in blocked state, decrement
				// by one each time sysClock is updated.
				// The execution of this statement does not
				// count for blocking period(?)
	// Recommended attributes:
	// program instructions array
	// instruction count of above array.
};


class Scheduler {
	private:
		unsigned int sysClock;
		unsigned int PCBcapacity;
		unsigned int PCBcount;
		unsigned int processCount;		
		const unsigned int priorityCount = 4;
		PCB *PCBtable;		
		PQ *ReadyState;
	public:
		step();		// If there is a running process, execute 
				// the current instruction of the running 
				// process. Update the System Clock, and 
				// the running process’s Time Used, 
				// time slice, and PC values accordingly. 
				// If the B instruction is executed, 
				// set the process’s block time accordingly 
				// and reduce the process’s priority.

		schedule();	// If there is not a running process, then 
				// dequeue one from the ReadyState queue.
				// If no process was ready to run, then do 
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

		swapIn();	// This retrieves the highest priority process 
				// from the ReadyState queue.  If such a 
				// process is dequeued, if the process’s time 
				// slice is 0, this sets the time slice 
				// according to the priority of the process. 
				// This then updates the CPU’s PID and time 
				// slice values.

		swapOut();	// This copies the CPU’s time slice value to 
				// the process’s time slice value and then 
				// enqueues the process to the ReadyState queue.

		processBlockedStates();
				// this updates the block time for every 
				// process in the blocked state.  If the 
				// block time of a process becomes 0, it 
				// enqueues the process to the ReadyState queue.

		runSystem();	// While there are allocated PCBs, this does 
				// the following, in order; step, schedule, 
				// process blocked states, and printState.

		kickOff();	// To get things going, the first process is 
				// created with the program found in a file 
				// by the name of “init.txt” and then 
				// runSystem is called.


};

#endif
