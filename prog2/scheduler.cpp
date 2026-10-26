// template created by Josiah

#include "scheduler.cpp"
#include "pidPQ.h"

// Empty PID value, use for initialization and empty checking
const unsigned int NULL_PID = 999;

//******************************************************************************

scheduler::scheduler() {
}

//******************************************************************************

scheduler::step() {
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

scheduler::swapIn() {

}
//******************************************************************************

scheduler::swapOut() {
}

//******************************************************************************

scheduler::processBlockedStates() {
}

//******************************************************************************

scheduler::runSystem() {
}

//******************************************************************************

scheduler::kickOff() {
}

//******************************************************************************
