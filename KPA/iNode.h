// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Author: Dylan Poirier
// This header file defines the iNode struct, which contains the necessary iNode
// metadata. A byte array shall store contiguous uninitialized iNode structs.
// A bitmap shall denote each iNode as free or used. Both the byte array and the
// btimap shall be part of the Block class, which will perform all necessary
// operations on iNodes.
//******************************************************************************

#ifndef INODE_H
#define INODE_H

#include <cstdint>
#include <ctime>

//******************************************************************************

// Represents fileTypes in readable format. Can be expanded for up to 255 types.
typedef enum fileType : uint8_t {
    DIR = 0,
    FILE_TYPE = 1
} fileType;

//******************************************************************************

// iNode struct contains iNode metadata. Directly accessible for ease-of-use.
typedef struct iNode {
    // Time fields representing last modified, last accessed, and birth time of
    // file respectively. cTime representes time of last iNode metadata change. 
    time_t mTime, aTime, bTime, cTime;
    
    uint32_t size; // File size in bytes. Updated by system calls when needed.
    uint32_t addr; // Address of first indirect block, set at initialization.
    uint16_t perms; // Unix-style, represented as a 3-digit octal number.
    uint16_t nLinks; // Counts hard links to file. Default: 1 (FILE), 2 (DIR).
    uint16_t uid, gid; // Stores the user and group ID of file owner.
    uint8_t gen; // Increments upon initialization. Number of reuses for iNode.
    fileType type; // Enumerated file type, 0 for directory and 1 for file.
} iNodeType;

//******************************************************************************

// Initializes iNode struct for use.
unsigned int iNodeInit(iNode &node);

//******************************************************************************

#endif