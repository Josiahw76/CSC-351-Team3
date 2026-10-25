// File updated by Josiah

#include <iostream>
#include "PQ.h"
#include "scheduler.h"

using namespace std;

//******************************************************************************

// Author: Josiah - Apr 7

PQ::PQ(unsigned int pqSize) {
    // Initialize our member variables
    this->pqSize = pqSize;
    qCount = 0;
    keys = new PCB*[pqSize];
}

//******************************************************************************

// Author: Josiah - Apr 7

PQ::~PQ() {
    // The PQ can only be populated DURING a buildTree operation,
    // so the responsibility of deleting KV's is elsewhere.
    delete [] keys;
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int PQ::parent(unsigned int index) const {
    // Given a child index, parent is found by (index - 1) / 2
    // Return zero if index is zero
    return index ? (index - 1) >> 1 : 0;
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int PQ::left(unsigned int index) const {
    // Left child is found by (index * 2) + 1
    return (index << 1) + 1; // Bit shift left for efficient doubling
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int PQ::right(unsigned int index) const {
    // Right child is found by (index * 2) + 2
    return (index << 1) + 2; // Bit shift for cheap multiplication
}

//******************************************************************************

// Bread-crumb

void PQ::printIt(unsigned int ind, unsigned int count) const {
    unsigned int i;

    if (ind < qCount) {
	    cout << "Level[" << (int)log2(ind + 1) << "]-> ";
	    for (i = ind; i < (ind+count); i++) {
	        if (i == qCount) {
		        break;
	        } else {
		        cout << "(" << keys[i]->priority << "," << keys[i]->PID << ") ";
	        }
	    }
	    cout << endl;
	    ind = left(ind);
	    printIt(ind, 2*count);
    }
}

//******************************************************************************

// Author: Josiah - Feb 19

void PQ::swap(PCB * &x, PCB * &y) {
    // Allocate a temporary variable so we can trade the values
    PCB *tempLoc = x;
    
    x = y; // Now overwrite x's address with y's kv
    y = tempLoc; // y now has x's original kv
}

//******************************************************************************

// Author: Dylan Poirier - 2/20

void PQ::bubbleUp(unsigned int index) {
    if (keys[index]->priority < keys[parent(index)]->priority) {
        // If a child's priority is less than its parent's, it must swap places
        
        // Swaps priority-values of the child and parent
        swap(keys[index], keys[parent(index)]);
        bubbleUp(parent(index)); // Continues bubbling up until heap is fixed
    }
}

//******************************************************************************
    
// Author: Josiah - Feb 19

void PQ::heapify(unsigned int index) {
    if (index < qCount) { // Don't do anything unless the given index is good

        // Get the left and right children for evaluation
        unsigned int L = left(index);
        unsigned int R = right(index);
        unsigned int smallest = 0; // Stores the candidate PID for comparison

        if (L < qCount && keys[L]->priority < keys[index]->priority) {
            // Determine whether the left child is less than the parent
            smallest = L;
        } else { 
            smallest = index; 
        }

        if (R < qCount && keys[R]->priority < keys[smallest]->priority) {
            // Now determine whether the right child is the smallest
            // If so, it should be the one switching with the parent
            smallest = R;
        }

        if (smallest != index) {
            // Swap if a child is smaller than the given index

            // Pass in the indices as pointers
            swap(keys[index], keys[smallest]);
        
            // Heapify the swapped index now
            heapify(smallest);
        }
    }
}

//******************************************************************************

// Author: Josiah - Apr 7

bool PQ::enq(PCB *kv) {
    // Normally we would check whether PQ is full, but it is actually
    // not possible to overfill the heap here. The PQ and freqs arrays
    // are initialized with the same PID, and we remove 2 before adding 1
    // during buildTree.

    // Toss it on the heap
    keys[qCount] = kv;
    qCount++;

    // Find where the PID belongs
    bubbleUp(qCount-1);
    return true;
}

//******************************************************************************

// Author: Dylan Poirier - 2/24
 
bool PQ::deq(PCB * &kv) {
    // Checks are made within huffman to avoid deq calls while qCount < 1
    // The PQ is also not exposed to the huffman user, so we control every call.
    kv = keys[0]; 
    swap(keys[0], keys[qCount - 1]);
    qCount--;
    heapify(0);
    return true;
}

//******************************************************************************

// Bread-crumb

void PQ::printIt() const {
    printIt(0, 1);
}

//******************************************************************************

// Author: Josiah - Apr 7

void PQ::clear() {
    // Clear doesn't do much. The PQ is not responsible for calling KV deletes,
    // and the heap is totally empty except during a buildTree.
    qCount = 0;
}

//******************************************************************************

// Author: Josiah - Feb 20

unsigned int PQ::count() const {
    return qCount;
}
