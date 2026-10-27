// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Author: Simeon Karne, Dylan Poirier
// The blocks class contains a bitmap and a byte array to store data in blocks
// and track which blocks are free. It is generalized to accomadate both 4096KB
// DataBlocks and 64B iNodes.
//******************************************************************************

#ifndef __BLOCKS_H
#define __BLOCKS_H

#include <cstdint>
#include <cstdio>
#include <ctime>

#include "iNode.h"

//******************************************************************************

class blocks {
    private:
        // Item count and size used to reconstruct data from backing store.
        uint32_t itemCount; // Length of bitmap and block array.
        uint32_t itemSize; // Size of each item in bytes, used for alignment.
        
        // Number of bytes needed for bitmap, including partially filled end.
        unsigned int bitmapByteCount;
        
        // Size of pool in bytes.
        unsigned int poolByteCount;
        
        // Tracks free and used items. Traverse the array until a byte < 255 is
        // found, then use bitwise operations to find the free bit.
        unsigned char *bitmap; 
        
        // Used to store the raw bytes of each item. Cast at runtime to align
        // with the size of blocks.
        unsigned char *pool;
        
    public:
        // Allocates sufficient memory for itemCount in bitmap and byte array.
        blocks(unsigned int itemCount, unsigned int itemSize);
        
        // Restores file system data from backing-store.
        blocks(FILE *store);

        // Deallocates dynamic memory to prevent leak.
        ~blocks();
        
        // Saves necessarry data to provided backing store. The itemCount and
        // itemSize go first, so that the length of the bitmap and byte array
        // can be determined.
        unsigned int save(fileDescriptor fd);
        
        // Return the first free item index, or -1 when full.
        int findFree();
        
        // Reserve a free item. Return its index, or -1 if none are free.
        int allocate();
        
        // Returns a pointer to pool array for use in other contexts.
        unsigned char* getPool() const;
        
        // Sets or clears specified bit in bitmap to mark as free or used.
        unsigned int BMsetBit(unsigned int bitID);
        unsigned int BMclearBit(unsigned int bitID);
        
};

//******************************************************************************

#endif
