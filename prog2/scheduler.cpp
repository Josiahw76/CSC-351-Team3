// template created by Josiah

#include "scheduler.cpp"
#include "pq/PQ.h"

//******************************************************************************

scheduler::scheduler() {
}

//******************************************************************************

scheduler::step() {
}

//******************************************************************************

scheduler::schedule() {
	// If there's no running process
	if (!CPU.timeLeft) {
		// Try to run a new process

		// If we have a process ready
		if (ReadyState.getCount()) {
		}

	} else if (PCBtable[])
}

//******************************************************************************

scheduler::printState() {
}

//******************************************************************************
//Author: Simeon K
void Scheduler::swapIn() {
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
	// Save the remaining CPU time, then return this process to the ready queue.
	unsigned int pid = CPU.runningPID; //Remember which process is leaving the CPU.
	
	//Save the remaining time for this process.
	PCBtable[pid].timeslice = CPU.timeLeft;

	// Put the process back in the ready queue.
	ReadyState->enq(&PCBtable[pid]);
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
