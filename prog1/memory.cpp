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

// Author: Josiah W

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

// Author: Dylan P - 9/17
memManager::memManager(unsigned int policy, unsigned int blockCount) {
    this->policy = policy; // Indicates which fit-policy will be used.
    
    // Enough nodes to fill storage with 3-block units.
    nodeCount = blockCount/3; 
    
    PIDlist = new list(nodeCount); // At most, one process per node.
    
    memRoot = new node(-1, 0, 0); // Root of DLL, linked to null on both sides.
    
    node *p = memRoot; // Initialize prev pointer for loop.
    node *n; // Declares next pointer for.
    
    for (unsigned int i = 1; i < nodeCount; i++) {
    // Iteratively creates the remaining number of needed nodes.
        // Creates new node, backward linked to its predecessor.
        p = new node(-1, 0, 0, p); 
        
        // Forward links the node before new node to the new node
        p->prev->next = p; 
    }
    
    p->next = NULL; // Forward links last node in DLL to NULL
}

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

// Author: Josiah
unsigned int countHoles() {
	int count = 0;
	int start1 = 0;
	int offset;
	node *p = memRoot;
	while (p != NULL) {	
		while (p->pid < 0 && p->next != NULL) { 
			p = p->next; 
		}
		// Moved forward: either next is null or current is allocated
		if (p->pid > 0) {
			offset = p->start - start1;
			if ((offset > 0) && (offset < 3)) {

				// Offset indicates unallocated blocks between neighboring nodes
				// between 0 and 3 means 1 or 2, which is a fragment
				count++;
			}
			start1 = p->start + p->length; // Calibrate start position
		}
	}	
	return count;
}

int memManager::allocMem(int process_id, int num_units) {}


// First fit policy (leftmost allocation):
// Kam - 9/17 (Bottom Line)
// Search through the list until we find the hole, and finishes when it's found.


void memManager::deallocMem(int process_id) {
	// This should just find all blocks with pid = process_id and
	// assign that value to -1
	bool rc = false;
	int index = process_id; // make it easier to read
	
	int pid = PIDlist->readAt(index); // translate index to process_id
	node *p = memRoot;

	while (p->pid != pid && p->next != NULL) {
		p = p->next;
	}
	// We've found the unwilling contestant!
	if (p->pid == pid) {
		p->pid = -1;
		PIDlist->deleteAt(index);
		rc = true;
	}
	return rc;
}

void memManager::checkLL(int process_id, int num_units) {}
// Kam - 9/17: This outputs the first hole that is big enough,
//while searching the list for a new hole.


void printIt(const) {}

