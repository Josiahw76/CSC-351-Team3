// CSC 255 Spring 2026 - Dr. Wheat.
// Program 6 PQ and minHeap
// Team 12: Josiah Winiecki and Dylan Poirier

#include <iostream>
#include <cmath>
#include "minHeap.h"

#include "common.h"

using namespace std;

//******************************************************************************

// Author: Josiah - Feb 19

minHeap::minHeap(int n) {

    // Initialize our member variables
    heapCapacity = n;
    heapCount = 0;
    keyValues = new KEY_VALUE[heapCapacity];
}

//******************************************************************************

// Author: Dylan Poirier - 2/20

minHeap::~minHeap() {
    delete [] keyValues; // Frees up dynamically allocated memory
    keyValues = NULL; // Ensures keyValues doesn't point to bogus data
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int minHeap::parent(unsigned int index) const {
    // Given a child index, parent is found by (index - 1) / 2
    // Return zero if index is zero
    return index ? (index - 1) >> 1 : 0;
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int minHeap::leftChild(unsigned int index) const {
    // Left child is found by (index * 2) + 1
    return (index << 1) + 1; // Bit shift left for efficient doubling
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int minHeap::rightChild(unsigned int index) const {
    // Right child is found by (index * 2) + 2
    return (index << 1) + 2; // Bit shift for cheap multiplication
}

//******************************************************************************

// Author: Josiah - Feb 19

void minHeap::swap(KEY_VALUE *x, KEY_VALUE *y) {
    // Allocate a temporary variable so we can trade the values
    KEY_VALUE tempLoc = *x;
    
    *x = *y; // Now overwrite x's address with y's kv
    *y = tempLoc; // y now has x's original kv
}

//******************************************************************************

// Author: Dylan Poirier - 2/20

void minHeap::bubbleUp(unsigned int index) {
    if (keyValues[index].key < keyValues[parent(index)].key) {
        // If a child's key is less than its parent's, it must swap places
        
        // Swaps key-values of the child and parent
        swap(&keyValues[index], &keyValues[parent(index)]);
        bubbleUp(parent(index)); // Continues bubbling up until heap is fixed
    }
}

//******************************************************************************
    
// Author: Josiah - Feb 19

void minHeap::heapify(unsigned int index) {
    if (index < heapCount) { // Don't do anything unless the given index is good

        // Get the left and right children for evaluation
        unsigned int L = leftChild(index);
        unsigned int R = rightChild(index);
        unsigned int smallest = 0; // Stores the candidate value for comparison

        if (L < heapCount && keyValues[L].key < keyValues[index].key) {
            // Determine whether the left child is less than the parent
            smallest = L;
        } else { 
            smallest = index; 
        }

        if (R < heapCount && keyValues[R].key < keyValues[smallest].key) {
            // Now determine whether the right child is the smallest
            // If so, it should be the one switching with the parent
            smallest = R;
        }

        if (smallest != index) {
            // Swap if a child is smaller than the given index

            // Pass in the indices as pointers
            swap(&keyValues[index], &keyValues[smallest]);
        
            // Heapify the swapped index now
            heapify(smallest);
        }
    }
}

//******************************************************************************

// Author: Josiah - Feb 19

bool minHeap::insert(KEY_VALUE kv) {
    bool rc = heapCount != heapCapacity;

    if (rc) {
        // Make sure heap isn't full first
    
        // Toss it on the heap
        keyValues[heapCount] = kv;
        heapCount++;

        // Find where the value belongs
        bubbleUp(heapCount-1);
    }
    
    return rc;
}

//******************************************************************************

// Author: Josiah - Feb 19

bool minHeap::removeByIndex(unsigned int index, KEY_VALUE &kv) {
    bool rc = false;
    
    if (index < heapCount) {
        // If the index is valid, we proceed with removal
        kv = keyValues[index];

	// Could check if we're removing last but cheaper to just swap anyway
        swap(&keyValues[index], &keyValues[heapCount - 1]);

        heapCount--; // We know the value is at the end now, so exclude it
        heapify(index); // Trickle the KV back down to the bottom now
        rc = true;
    }
    return rc;
}

//******************************************************************************

// Author: Dylan Poirier - 2/21

bool minHeap::removeByKey(int key, KEY_VALUE &kv) {
    unsigned int index;
    // Finds the first value of key and stores its location in index
    bool rc = getIndex(key, index);
    
    if (rc) {
        // Only attempt to remove if a matching key was found
        
        kv = keyValues[index]; // Passes back key-value prior to removal
        // Replaces key-value with the last in the heap
        keyValues[index] = keyValues[heapCount - 1];
        heapCount--; // Shortens the list to account for removed key-value
        
        // Checks to see if heap still follows the rules
        if (keyValues[index].key < keyValues[parent(index)].key) {
            // Key is smaller than parent, requires bubble up
            bubbleUp(index);
        } else if (keyValues[index].key > keyValues[leftChild(index)].key ||
                   keyValues[index].key > keyValues[rightChild(index)].key) {
            // Key is bigger than one of it's children, requires trickle down
            heapify(index);
        }
    }
    
    return rc; // Only false if a matching key was not found
}

//******************************************************************************

// Author: Dylan Poirier - 2/20

void minHeap::clear() {
    heapCount = 0; // "Empties" heap
    
    delete [] keyValues; // Frees memory space
    
    if (heapCapacity != 0) {
        // Only bothers creating new array if it will have a non-zero capacity
        keyValues = new KEY_VALUE[heapCapacity];
    } else {
        // Otherwise, since capacity is zero, clears "keyValues"
        keyValues = NULL;
    }
}

//******************************************************************************

bool minHeap::integrity() const {
    bool rc = true;

    for (unsigned int index = 0; index < heapCount; index++) {
        unsigned int lc = leftChild(index);
        unsigned int rc = rightChild(index);

        if (rc != lc + 1) {
            rc = false;
            cout << "Integrity check fails for lc and rc" << endl;
            break;
        }

        if (lc < heapCount) {
            if (keyValues[index].key > keyValues[lc].key) { 
                rc = false;
                cout << "Integrity check fails parent not <= than lc" << endl;
                printIt();
                exit(-1);
                break;
            }

            if (rc < heapCount) {
            if (keyValues[index].key > keyValues[rc].key) {
                rc = false;
                cout << "Integrity check fails parent not <= than rc" << endl;
                break;
            }
            }
        }
    }
    return rc;
}

//******************************************************************************

// Author: Dylan Poirier - 2/21

bool minHeap::getIndex(int key, unsigned int &returnIndex) const {
    bool rc = false; // Default return code if key is not found
    
    for (unsigned int i = 0; i < heapCount; i++) {
        if (keyValues[i].key == key) {
            // Matching key is found
            
            returnIndex = i; // Returns index of matching key
            rc = true; // Reports successful find
            break; // Key was found, no need to keep searching
        }
    }
    
    return rc; // Reports succssful or failure to find matching key
}

//******************************************************************************

// Author: Dylan Poirier - 2/21

bool minHeap::readByIndex(unsigned int index, KEY_VALUE &kv) const {
    bool rc = index < heapCount; // Checks if index is valid
    
    if (rc) {
        // Index is valid, find value to return
        
        kv = keyValues[index]; // Passes key-value back through kv
    }
    
    return rc; // Only false for invalid indices
}

//******************************************************************************

void minHeap::printIt(unsigned int ind, unsigned int count) const {
    if (ind < heapCount) {
    cout << "Level[" << (int)log2(ind + 1) << "]-> ";

    for (unsigned int i = ind; i < (ind+count); i++) {
        if (i == heapCount) {
        break;
        } else {
        cout << "(" << keyValues[i].key << "," << keyValues[i].value << ") ";
        }
    }

    cout << endl;
    ind = leftChild(ind);

    printIt(ind, 2*count);
    }
}

//******************************************************************************

void minHeap::printIt() const {
    cout << "The Heap Count is    " << heapCount << endl;
    cout << "The Heap Capacity is " << heapCapacity << endl;

    printIt(0, 1);
}

//******************************************************************************

// Author: Josiah - Feb 19

unsigned int minHeap::getCount() const {
    return heapCount;
}

//******************************************************************************

// Author: Dylan Poirier - 2/21

unsigned int minHeap::getCapacity() const {
    return heapCapacity;
}
