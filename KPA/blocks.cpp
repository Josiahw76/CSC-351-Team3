// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Author: Dylan Poirier
//******************************************************************************

#include <iostream>
#include "blocks.h"

//******************************************************************************

// Author: Dylan P
blocks::blocks(unsigned int itemCount, unsigned int itemSize) {
    this->itemCount = itemCount;
    this->itemSize = itemSize;
    
    bitmapByteCount = (itemCount/8) + ((itemCount%8) > 0);
    poolByteCount = itemCount * itemSize;
    
    // Allocate bytes for 1 bit per item, while setting them all to zero.
    bitmap = new unsigned char[bitmapByteCount]{};
    pool = new unsigned char[poolByteCount]; // Allocate bytes for items.
    
    if (itemCount % 8 > 0) {
    // If there are lefetover bits they need to be filled.
        unsigned char mask = (255 << itemCount%8);
        bitmap[bitmapByteCount - 1] = mask;
    }
}

//******************************************************************************

// Author: Dylan P
blocks::blocks(FILE *store) {
    // Reads the first 4 bytes of store to get number of items.
    fread(&itemCount, sizeof(itemCount), 1, store);
    
    // Reads the next 4 bytes of store, which is where itemSize is stored.
    fread(&itemSize, sizeof(itemSize), 1, store);
    
    bitmapByteCount = (itemCount/8) + ((itemCount%8) > 0);
    poolByteCount = itemCount * itemSize;
    
    // Allocate bytes for 1 bit per item.
    bitmap = new unsigned char[bitmapByteCount];
    pool = new unsigned char[poolByteCount]; // Allocate bytes for items.
    
    fread(bitmap, 1, bitmapByteCount, store); // Fills in bitmap.
    
    unsigned char byte; // Used for handling bytes in the for-loop.
    unsigned int itemBytePos; // Indexes position of desired item in pool.

    for (unsigned int i = 0; i < bitmapByteCount; i++) {
    // Traverses each byte in the bitmap, looking for allocated blocks.
        byte = bitmap[i]; 
        
        if (byte > 0) {
        // A byte value greater than 0 indicates at least one bit is set.
            
            for (unsigned int j = 0; j < 8; j++) {
            // Traverses each bit in the current byte, looking for set bits.
            
                // Bit pos + byte pos * 8 gives item index. Multiply by item
                // size gets byte offset. If the index is out of bounds, it
                // is because all items are accounted for, so exit loop.
                itemBytePos = (j + i*8) * itemSize;
                if (itemBytePos >= poolByteCount) {
                    break;
                }
                
                if ((byte >> j) & 1) {
                // True when bit at pos j is set.
                    
                    // Reads the next item from store into the correct byte
                    // position in pool.
                    fread(pool + itemBytePos, 1, itemSize, store);
                }
            }
        }
    }
}

//******************************************************************************

// Author: Dylan P
blocks::~blocks() {
    // Frees the dynamically allocated memory to avoid leak.
    delete [] bitmap;
    delete [] pool;
    
    bitmap = NULL; // Ensures pointers don't access garbage.
    pool = NULL; // Ensures pointers don't access garbage.
}

//******************************************************************************

// Author: Dylan P
unsigned int blocks::save(FILE *store) {    
    unsigned int rc = store != NULL; // Return condition 1 if file is valid.
    unsigned int savedItemCount = 0; // Tracks number of saved items for check.
    size_t bw = 0; // Number of items written to the file. Checks success.
    
    
    if (rc) {
        // Writes number of items for length of store. Takes 4 bytes of space.
        bw += fwrite(&itemCount, sizeof(itemCount), 1, store);
        
        // Gives length of each block, used with itemCount for size. 4 bytes.
        bw += fwrite(&itemSize, sizeof(itemSize), 1, store);
        
        // Stores bitmap 1 byte at a time. Adds bitmapByteCount to bw.
        bw += fwrite(bitmap, 1, bitmapByteCount, store);
        
        unsigned char byte; // Used for handling bytes in the for-loop.
        unsigned int itemBytePos; // Indexes position of desired item in pool.
        
        for (unsigned int i = 0; i < bitmapByteCount; i++) {
        // Traverses each byte in the bitmap, looking for allocated blocks.
            byte = bitmap[i]; 
            
            if (byte > 0) {
            // A byte value greater than 0 indicates at least one bit is set.
                
                for (unsigned int j = 0; j < 8; j++) {
                // Traverses each bit in the current byte, looking for set bits.
                
                    // Bit pos + byte pos * 8 gives item index. Multiply by item
                    // size gets byte offset. If the index is out of bounds, it
                    // is because all items are accounted for, so exit loop.
                    itemBytePos = (j + i*8) * itemSize;
                    if (itemBytePos >= poolByteCount) {
                        break;
                    }
                    
                    if ((byte >> j) & 1) {
                    // True when bit at pos j is set.
                        
                        // Writes the item at itemBytePos from pool to store.
                        bw += fwrite(pool + itemBytePos, 1, itemSize, store);
                        savedItemCount++; // Accounts for saved item.
                    }
                }
            }
        }
                                    
        
        // If everything was written successfully, bw should equal total items.
        rc = bw == (1 + 1 + bitmapByteCount + savedItemCount*itemSize);
    }
    
    return rc;
}

//******************************************************************************

// Author: Dylan P
unsigned char* blocks::getPool() const {
    return pool;
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


