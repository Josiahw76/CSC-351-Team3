#ifndef __MINHEAP_H
#define __MINHEAP_H

// CSC 255 Spring 2026 - Dr. Wheat.
// Program 6 PQ and minHeap
// Team 12: Josiah Winiecki and Dylan Poirier

//******************************************************************************

#include "common.h"

//******************************************************************************

class minHeap {
    private:
        keyValue **keys;
        unsigned int heapCapacity;
        unsigned int heapCount;

        // Josiah - Copied these functions from lecture notes
        unsigned int parent(unsigned int index) const; // Get parent index
        unsigned int leftChild(unsigned int index) const; // Get left child
        unsigned int rightChild(unsigned int index) const; // Get right child

        // This function was provided
        void printIt(unsigned int ind, unsigned int count) const;

        // Josiah
        // Switches the values contained in x and y's addresses.
        //
        // Note: to call this function with index numbers, write it like so:
        // swap(&array[index_x], &array[index_y])
        void swap(keyValue * &x, keyValue * &y);

        // Dylan
        // If the key-value pair at index is smaller than its parent, 
        // recursively swaps key-value pair at index with its parent until it is
        // bigger than its parent
        void bubbleUp(unsigned int index);

        // Josiah
        // Compares the given index to the child KVs, and swaps with the
        // lesser of the two children if needed, trickling to the bottom of
        // the heap using recursion.
        //
        // This is one of the primary methods used to maintain
        // the minHeap properties.
        void heapify(unsigned int i);

    public:

        // Josiah
        // Initializes the member variables and allocates an array of n size
        minHeap(int n = 100); 

        // Dylan
        // Removes dynamic storage, prevents memory hole
        ~minHeap(); 

        // Josiah
        // Places the kv at the bottom of the heap, calling bubbleUp() to
        // find the proper position within the heap.
        bool insert(keyValue *kv);

        // Josiah
        // Removes the kv at the given index from the heap.
        // First swaps the given index with the last index, decrements
        // heapCount, then calls heapify on the kv that was swapped.
        bool removeByIndex(unsigned int index, keyValue * &kv);

        // Dylan
        // Removes the first occurrence of key in heap. Updates heapCount if
        // necessary, returns true if key was found and removed, else false
        bool removeByKey(int key, keyValue * &kv);
        
        // Dylan
        // Empties the heap and reallocates a new area with heapCapacity size
        void clear();

        // This function was provided
        bool integrity() const; // Ensure the heap obeys heap properties

        // Dylan
        // Finds first occurrence of key in heap, returns the index of that key.
        // Returns true if matching key is found, false otherwise.
        bool getIndex(int key, unsigned int &returnIndex) const;
        
        // Dylan
        // Returns the key-value pair at given index and true. Returns false if
        // the given index is outside of the heap.
        bool readByIndex(unsigned int index, keyValue * &kv) const;

        // This function was provided
        void printIt() const;

        // Josiah
        // Returns the number of items in the heap
        unsigned int getCount() const;

        // Dylan
        // Returns the capacity of heap
        unsigned int getCapacity() const;
};

#endif
