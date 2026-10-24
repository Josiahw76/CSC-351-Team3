// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Author: Simeon Karne, Dylan Poirier
// The blocks class contains a bitmap and a byte array to store data in blocks
// and track which blocks are free. It is generalized to accomadate both 4096KB
// DataBlocks and 64B iNodes.
//******************************************************************************

#ifndef BLOCKS_H
#define BLOCKS_H

#include <cstdint>
#include <ctime>

//******************************************************************************

class blocks {
    private:
        // Item count and size used to reconstruct data from backing store.
        unsigned int itemCount; // Length of bitmap and block array.
        unsigned int itemSize; // Used for allignment within the block array.
        
        // Number of bytes needed for bitmap, plus one extra if leftovers exist.
        unsigned int bitmapByteCount;
        
        // Tracks free and used items. Traverse the array until a byte < 255 is
        // found, then use bitwise operations to find the free bit.
        char *bitmap; 
        
        // Used to store the raw bytes of each item. Cast at runtime to align
        // with the size of blocks.
        char *pool;
        
    public:
        // Allocates sufficient memory for itemCount in bitmap and byte array.
        blocks(unsigned int itemCount, unsigned int itemSize);
        
        // Deallocates dynamic memory to prevent leak.
        ~blocks();
        
        char* getPool() const;
        
        // Sets or clears specified bit in bitmap to mark as free or used.
        unsigned int BMsetBit(unsigned int bitID);
        unsigned int BMclearBit(unsigned int bitID);
        // Return the first free item index, or -1 when full.
        int findFree();
        
        //Reserve a free item. Return its index, or -1 if none are free.
        int allocate();
};

//******************************************************************************

#endif