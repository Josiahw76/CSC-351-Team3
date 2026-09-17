#include "memory.h"
#include <iostream>

//******************************************************************************
//				Node Function				      //
//******************************************************************************

node::node(int pid, unsigned int start, unsigned int length, node *prev = NULL, node *next = NULL) {
	this->pid = pid;
	this->start = start;
	this->length = length;
	this->prev = prev;
	this->next = next;
}

//******************************************************************************
//				List Functions				      //
//******************************************************************************

// Josiah - I'm just gonna write this out
//
// I did some quick testing and this works just fine.
// Note that it is possible to allocate a list of size zero, just
// nothing happens when you do so it's pointless.

list::list(unsigned int listCapacity) {
	this->listCapacity = listCapacity;
	listCount = 0;

	// If capacity is zero, don't bother allocating anything
	if (listCapacity > 0) {
		a = new unsigned int[listCapacity];
	} else {
		a = NULL;
	}
}

list::~list() {
	if (a != NULL) { delete [] a; } // If a exists, delete it.
}

bool list::add(int val) {
	bool rc = listCount < listCapacity;
	if (rc) {
		a[listCount] = val;
		listCount++;
	}
	return rc;
}

bool list::deleteAt(unsigned int index) {
	bool rc = (index > 0 && index <= listCapacity);
	if (rc) {
		// decrement before to account for zero indexing
		listCount--;
		a[index] = a[listCount];
	}
	return rc;
}

int list::readAt(unsigned int index) {
	int rc = -1; // return -1 if read failed for any reason
	if (index > 0 && index <= listCapacity) {
		rc = a[index];
	}
	return rc;
}


void list::printIt() const {
	for (unsigned int i = 0; i < listCount; i++) {
		std::cout << a[i] << std::endl;
	}
}

unsigned int list::getCount() {
	return listCount;
}

//******************************************************************************
//				memManager Functions			      //
//******************************************************************************

memManager::memManager(unsigned int policy, unsigned int blockCount) {}

memManager::~memManager() {
	// can't really do a recursive deletion here, so
	// conditional loop it is.
	node *p = first;
	while (p->next != NULL) {
		p = p->next;
		delete p->prev;
	}
	delete p;
}

int memManager::allocMem(int process_id, int num_units) {}


// First fit policy (leftmost allocation):
// Search through the list until 


void memManager::deallocMem(int process_id) {
	// This should just find all blocks with pid = process_id and
	// assign that value to -1
}

void memManager::checkLL(int process_id, int num_units) {}
// Kam:(I think this is good.) We know this outputs the first hole that is big enough, while searching the list for a new hole
void printIt(const) {}


// Policy definitions below:

/*
First fit. Allocate the first hole that is big enough. Searching can start either at the beginning of the set of holes or at the location where the previous first-fit search ended. We can stop searching as soon as we find a free hole that is large enough.

Best fit. Allocate the smallest hole that is big enough. We must search the entire list, unless the list is ordered by size. This strategy produces the smallest leftover hole.

Worst fit. Allocate the largest hole. Again, we must search the entire list, unless it is sorted by size. This strategy produces the largest leftover hole, which may be more useful than the smaller leftover hole from a best-fit approach.

   */
