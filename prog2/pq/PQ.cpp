// CSC 255 Spring 2026 - Dr. Wheat
// Program 6 PQ
// Team 12: Josiah Winiecki and Dylan Poirier

//******************************************************************************

#include <iostream>
#include "minHeap.h"
#include "PQ.h"
#include "common.h"

using namespace std;

//******************************************************************************

// Author: Josiah - Feb 20

PQ::PQ(unsigned int pqCapacity) {
    // Allocate the Heap
    theHeap = new minHeap(pqCapacity);
}

//******************************************************************************

// Author: Dylan Poirier - 2/23

PQ::~PQ() {
    theHeap->~minHeap(); // The dynamic memory is all in min-heap
}

//******************************************************************************

// Author: Josiah - Feb 20

bool PQ::enq(keyValue *kv) {
    // Heap's insert already does everything we need it to
    return theHeap->insert(kv);
}

//******************************************************************************

// Author: Dylan Poirier - 2/24

bool PQ::deq(keyValue * &kv) {
    return theHeap->removeByIndex(0, kv); // The first kv is always the least
}

//******************************************************************************

// Author: Josiah - Feb 20

void PQ::printIt() const {
    theHeap->printIt(); // Nothing wrong with the print func we already have
}

//******************************************************************************

// Author: Dylan Poirier - 2/24

void PQ::clear() {
    theHeap->clear();
}

//******************************************************************************

// Author: Josiah - Feb 20

unsigned int PQ::getCount() const {
    return theHeap->getCount(); // Just return heapCount
}

//******************************************************************************

// Author: Dylan Poirier - 2/24

unsigned int PQ::getCapacity() const {
    return theHeap->getCapacity();
}
