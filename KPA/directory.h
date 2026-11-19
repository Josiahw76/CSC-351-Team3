// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Authors: Dylan Poirier
// This header file is for the Directory class. It defines a dirEntry struct and
// contains operations for managing and using directories.
//******************************************************************************

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <cstdint>
#include <string>
#include <cstring>
#include <ctime>
#include "blocks.h"

//******************************************************************************

typedef struct dirEntry {
    char filename[32];
    unsigned int iNodeIndex;
} dirEntry;
    
//******************************************************************************

class directory {    
    private:
        blocks *iNodes;
        blocks *dataBlocks;
        
        
    
        int resolvePath(std::string path, std::string &target);
        
        int search(unsigned int dirIndex, std::string target);
        
        // Initializes iNode struct for use.
        unsigned int iNodeInit(iNode &node, blocks *dataBlocks);
        
        unsigned int stringToChars(std::string src, char *dst);
        
//******************************************************************************

        
    public:
        directory(blocks *nodes, blocks *data);
        
        unsigned int mkdir(std::string path, 
                                  blocks *iNodes, blocks *dataBlocks);
           
       
};

//******************************************************************************

#endif