#ifndef INODE_H
#define INODE_H

#include <cstdint>
#include <ctime>

//******************************************************************************

// Represents fileTypes in readable format. Can be expanded for up to 255 types.
typedef enum fileType : uint8_t {
    DIR = 0,
    FILE = 1
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


#endif