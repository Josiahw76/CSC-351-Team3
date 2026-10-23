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


