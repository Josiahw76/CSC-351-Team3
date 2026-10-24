// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Author: Dylan Poirier
//******************************************************************************

#include <iostream>
#include "blocks_simeon.h"

//******************************************************************************

// Author: Dylan P / Simeon K
blocks::blocks(unsigned int itemCount, unsigned int itemSize) { //SK
    this->itemCount = itemCount;
    this->itemSize = itemSize;
    
    bitmapByteCount = (itemCount / 8) + ((itemCount % 8) >0); // Adds 1 if there are leftover bits. SK ()
    bitmap = new char[bitmapByteCount]; // Allocates bytes for 1 bit per item.
    pool = new char[itemCount * itemSize]; // Allocates bytes to store items.
    //Starts with all items marked free.
    for (unsigned int i = 0; i < bitmapByteCount; i++) {
        bitmap[i] = 0; // Initializes all bits to 0 (free).
    }
}

//******************************************************************************

blocks::~blocks() {
    //Release both arrays allocated by the contructor.
    delete[] bitmap;
    delete[] pool;
}

//******************************************************************************

// Author: Dylan P
char* blocks::getPool() const {
    return pool; // Returns pointer to pool so that other classes can use pool.
}

//******************************************************************************

// Author: Dylan P
unsigned int blocks::BMsetBit(unsigned int bitID) {
    unsigned int rc = bitID < itemCount; // Success (1) if bitID is in bounds.
    
    if (rc) {
    // Continue only if bitID is valid.
        unsigned int byte = bitID / 8; // Floor division gets the desired byte.
        unsigned int bit = bitID % 8; // Remainder gives specific bit in byte.
        
        unsigned char oldByte = bitmap[byte]; // Fetches appropriate byte.
        unsigned char newByte = oldByte | (1 << bit); // Insert 1 in position.
        
        bitmap[byte] = newByte; // Puts the new byte with set bit back in place.
    }
    
    return rc; // If bitID is out of bounds, rc will be 0, indicating failure.
}

//******************************************************************************

// Author: Dylan P
unsigned int blocks::BMclearBit(unsigned int bitID) {
    unsigned int rc = bitID < itemCount; //Success (1) if bitID is in bounds.
    
    if (rc) {
    // Continue only if bitID is valid.
        unsigned int byte = bitID / 8; // Floor division gets the desired byte.
        unsigned int bit = bitID % 8; // Remainder gives specific bit in byte.
        
        unsigned char oldByte = bitmap[byte]; // Fetches appropriate byte.
        unsigned char newByte = oldByte & ~(1 << bit); // Insert 0 in position.
        
        bitmap[byte] = newByte; // Puts the new byte with set bit back in place.
    }
    
    return rc; // If bitID is out of bounds, rc will be 0, indicating failure.
}

//******************************************************************************

// Author: Simeon K
int blocks::findFree() 
{
    for (unsigned int byteIndex = 0;
        byteIndex < bitmapByteCount;
        byteIndex++)
    {
        // Read the byte as 0–255, even if plain char is signed.
        unsigned char currentByte = bitmap[byteIndex];

        if (currentByte != 255)
        {
            unsigned int bitPosition = 0;

            while (bitPosition < 8 && ((currentByte >> bitPosition) &1) == 1)
            {
                bitPosition++;
            }

            unsigned int itemIndex = byteIndex * 8 + bitPosition;

            //Ignore unused padding bits in the last byte of the bitmap.
            if (itemIndex < itemCount)
            {
                return itemIndex;

            }
        }
    }

    return -1; // No free item was found.

}

//******************************************************************************

// Author: Simeon K
int blocks::allocate()
{
    int index = findFree();

    //Stop if there are no free items.
    if (index == -1)
    {
        return -1;
    }

    // Mark the item as used so another allocation cannot use it.
    BMsetBit(index);
    return index;
}