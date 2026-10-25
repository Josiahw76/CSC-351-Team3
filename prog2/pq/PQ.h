#ifndef __PQ_H
#define __PQ_H

// CSC 255 Spring 2026 - Dr. Wheat
// Program 6 PQ
// Team 12: Josiah Winiecki and Dylan Poirier

//******************************************************************************

#include "minHeap.h"
#include "common.h"

//******************************************************************************

class PQ {
    private:
        minHeap *theHeap;

    public:

        // Josiah
        // Constructs the PQ by allocating a heap object
        PQ(unsigned int pqCapacity = 100); 

        // Dylan
        // Removes dynamically allocated memory
        ~PQ(); 

        // Josiah
        // Inserts the kv using heap insert.
        // Queue property is maintained by bubbleUp within insert.
        bool enq(KEY_VALUE kv);

        // Dylan
        // Removes the kv with the least key from the PQ and passes it 
        // back through kv. Returns false if the PQ is empty.
        bool deq(KEY_VALUE &kv);

        // Josiah
        // Calls the heap's print function
        void printIt() const;

        // Dylan
        // Empties the PQ
        void clear();

        // Josiah
        // Returns the count of items within the heap
        unsigned int getCount() const;

        // Dylan
        // Returns the capacity of the PQ
        unsigned int getCapacity() const;
};

#endif
