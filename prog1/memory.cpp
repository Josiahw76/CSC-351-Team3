#include "memory.h"


//******************************************************************************
//				Node Function				      //
//******************************************************************************

node::node(int pid, unsigned int start, unsigned int length, node *prev = NULL, node *next = NULL) {
	this.pid = pid;
	this.start = start;
	this.length = length;
	this.prev = prev;
	this.next = next;
}

//******************************************************************************
//				List Functions				      //
//******************************************************************************

list::list(unsigned int listCapacity) {}


list::~list() {}


bool list::add(int val) {}


bool list::deleteAt(unsigned int index) {}


int list::readAt(unsigned int index) {}


void list::printIt() {}


unsigned int list::getCount() {}

//******************************************************************************
//				memManager Functions			      //
//******************************************************************************

memManager::memManager(unsigned int policy, unsigned int blockCount) {}

memManager::~memManager() {}

int memManager::allocMem(int process_id, int num_units) {}

void memManager::deallocMem(int process_id) {
	// This should just find all blocks with pid == process_id and
	// assign that value to -1
}

void memManager::checkLL() {}

void printIt() {}


