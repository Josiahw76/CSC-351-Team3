// template created by Josiah

#include "scheduler.cpp"
#include "pidPQ.h"

// Empty PID value, use for initialization and empty checking
const unsigned int NULL_PID = 999;

//******************************************************************************

scheduler::scheduler() {
}

//******************************************************************************

// Author: Josiah

void scheduler::step() {
	// If current running process: execute process

	if (CPU.runningPID != NULL_PID) {
		// Now, execute current instruction.
		// First get the PCB
		PCB *activeProcess;
		searchByPID(CPU.runningPID, activeProcess);

		switch activeProcess->

	}
}

//******************************************************************************

// Author: Josiah W

void scheduler::schedule() {
	// Basket variable for returns
	PCB *newActive;	

	// If there's no running process
	if (CPU.PID == NULL_PID) {
		// Try to run a new process

		// If we have a process ready
		if (ReadyState.getCount() > 0) {
			// Take the first in line
			ReadyState->deq(newActive);

			// Write the values into our active slot
			CPU.runningPID = newActive->PID;
			CPU.timeLeft = newActive->timeslice;
		}

		// How do we check if the pid is blocked?
		// need a searchByPID function to return the actual pointer
		// to the process information
	} else {
		searchByPID(CPU.PID, newActive);
		if (newActive->blockTime > 0) {
			// Active process is blocked, so trade it out.
			swapOut();
			swapIn();
		}
	}
}

//******************************************************************************

// This is heavy formatting.
scheduler::printState() {
}

//******************************************************************************

// Author: Josiah W
// Basic lookup function to get a process's information based on the PID

bool scheduler::searchByPID(unsigned int PID, PCB * &pcb) {
	bool rc = false;
	for (unsigned int i = 0; i < PCBcount; i++) {
		if (PCBtable[i].PID == PID) {
			pcb = &PCBtable[i];
			rc = true;
			break;
		}
	}
	return rc;
}

//******************************************************************************

//Author: Simeon K

void scheduler::swapIn() {

	// If no process is ready, there is nothing to swap in.
	if (ReadyState->count() == 0) {
		return;
	}

	// Get the highest pririty process from ready queue.
	PCB *process;
	ReadyState->deq(process);

	// Give a new time slice if the previous one was used up.
	if (process->timeslice == 0) {
		process->timeslice = 1 << process->priority; // 2^priority
	}

	// Load the process in the CPU.
	CPU.runningPID = process->PID;
	CPU.timeLeft = process->timeslice;
}

//******************************************************************************

//Author: Simeon K

void Scheduler::swapOut() {
	// Save the remaining CPU time, then return this process to the 
	// ready queue.
	unsigned int pid = CPU.runningPID; // Remember which process is 
					   // leaving the CPU.
	
	//Save the remaining time for this process.
	PCBtable[pid].timeslice = CPU.timeLeft;

	// Put the process back in the ready queue.
	ReadyState->enq(&PCBtable[pid]);
	
}

//******************************************************************************

// Author: Josiah W

void scheduler::processBlockedStates() {
	for (unsigned int i = 0; i < PCBcount; i++) {
		if (PCBtable[i].blockTime > 0) {
			if (!--PCBtable[i].blockTime) {
				ReadyState->enq(&PCBtable[i]);
			}
		}
	}
}

//******************************************************************************

scheduler::runSystem() {
}

//******************************************************************************

scheduler::kickOff() {
}

//******************************************************************************
