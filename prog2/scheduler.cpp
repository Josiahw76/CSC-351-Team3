// template created by Josiah

#include "scheduler.cpp"
#include "pidPQ.h"
#include <iostream>

using namespace std;

// Empty PID value, use for initialization and empty checking
const unsigned int NULL_PID = 999;

// HELPER FUNCTIONS - Josiah W
int reconstruct_int(char *str) {
    char a;
	int count = 0;
	int reconstructed_int = 0;

	while ((a = str[count]) != '\0') {
   	 	reconstructed_int *= 10;
    	reconstructed_int += a - '0';
    	count++;
		// Shift current val one left and append the number.
		// for 15: 00 <- 01 <- 10 <- 15
	}
	return reconstructed_int;
}
void parseInstruction(char *instr, char * &buf) {
	unsigned int size = sizeof(instr) - 2;
	delete [] buf;
	buf = new char[size];

	for (unsigned int i = 2; i < size; i++) {
		buf[i - 2] = instr[i];
	}
}

//******************************************************************************

// Author: Simeon K

scheduler::Scheduler() {
	// Initialize the clock and process counts.
	sysClock = 0;
	PCBcount = 0;
	processCount = 0;

	// No process is running yet.
	CPU.runningPID = NULL_PID;
	CPU.timeLeft = 0;

	// Create space for the process table and ready queue.
	PCBcapacity = 10;
	PCBtable = new PCB[PCBcapacity];
	ReadyState = new PQ(PCBcapacity);

	//Initialize each PCB entry to NULL_PID so we can tell if it's allocated or not.
	for (unsigned int i = 0; i < PCBcapacity; i++) {
		PCBtable[i].PID = NULL_PID;
		PCBtable[i].blockTime = 0;
	}
}

//******************************************************************************

// Author: Josiah W

void scheduler::step() {
	// If current running process: execute process

	if (CPU.runningPID != NULL_PID) {
		// Now, execute current instruction.
		// First get the PCB
		PCB *currentPID;
		searchByPID(CPU.runningPID, currentPID);

		char *buf;
		parseInstruction(currentPID->instructions[currentPID->PC], buf);

		switch (currentPID->instructions[currentPID->PC][0]) {

			case 'S':
			programValue = reconstructInt(buf);
			break;

			case 'A':
			programValue += reconstructInt(buf);
			break;
	
			case 'D':
			programValue -= reconstructInt(buf);
			break;
	
			case 'F':
			// Create a new process that is an exact copy of current
			// process
			PCBtable[PCBcount] = *currentPID;

			// Update values to be unique process
			PCBtable[PCBcount].PID = PCBcount;
			PCBtable[PCBcount].PPID = currentPID->PID;
			PCBtable[PCBcount].PC++;
			PCBtable[PCBcount].startTime = sysClock;
			PCBtable[PCBcount].blockTime = 0;

			ReadyState->enq(&PCBtable[PCBcount]);
			break;

			case 'B':
			currentPID->blockTime = (unsigned int)reconstructInt(buf);
			break;

			case 'R':
			// I think this is overwrite all instructions with
			// whatever is in the fname. No clue if we have a func
			// for that already
			break;

			case 'E':
			currentPID->PID = NULL_PID;
			CPU.runningPID = NULL_PID;
			CPU.timeLeft = 0;
			break;
		}
		sysClock++;
		CPU.timeslice--;
		currentPID->timeUsed++;
		currentPID->PC++;
		if ((CPU.timeslice == 0) && (currentPID->PID != NULL_PID)) {
			if (currentPID->priority < 3) {
				currentPID->priority++;
			}
			swapOut();
		}
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

// Author: Josiah W

scheduler::printState() {

	char *separator = "*" * 65;

	PCB p = PCBtable[CPU.PID];
	cout << "CPU PID = " << CPU.runningPID << "  "
		<< "CPUT timeslice = " << CPU.timeslice << endl;
	// It's not pretty, but this should be every variable
	printf("%-6s %-6s %-4s %-4s %-8s %-12s %-10s %-10s\n",
       "pid", "ppid", "pri", "pc", "value",
       "timeStart", "timeslice", "timeUsed");

	printf("%-6i %-6i %-4i %-4i %-8i %-12i %-10i %-10i\n",p.PID,p.PPID,
		p.priority,p.PC,p.value,p.startTime,p.timeslice,p.timeUsed);

	printf("%s",separator);
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

// Author: Kam E

void scheduler::runSystem() {
	// So run system preps things in order with: schedule,
	//  step, process and states.
(I'm still writing. Just resetting.)
	
}

//******************************************************************************

// Author: Kam E

void scheduler::kickOff() {
	// kickOff "kicks off" if you will. 
	// It loads this program
	// Then makes a small file called init.txt
	//  Now, runSystem is called, cause I think its the main point
	// In this scheduler class.
(I'm still writing. Just resetting.)
   
}

//******************************************************************************
