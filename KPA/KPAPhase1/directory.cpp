// CSC 351 Fall 2026 – Dr. Stephen Wheat
// KPA Phase 1 - File System Simulation 
// Team 2: Kam E, Simeon K, Dylan P, Josiah W

//******************************************************************************
// Authors: Dylan Poirier
//******************************************************************************

#include "directory.h"
#include "blocks.h"


using namespace std;

//******************************************************************************

int directory::resolvePath(string path, string &target) {
    
}        
        
//******************************************************************************

// Author: Dylan P
unsigned int directory::iNodeInit(iNode &node, blocks *data) {
    unsigned int rc = data->findFree() == -1 ? 0 : 1;
    
    if (rc) {
        time_t currTime = time(nullptr);
        node.mTime = node.aTime = node.bTime = node.cTime = currTime;
        
        node.size = 2 * sizeof(dirEntry);
        
        node.addr = data->allocate();
        
        node.perms = 0777;
        
        node.nLinks = 2;
        
        node.gen++;
        
        node.type = DIR;
    }
    
    return rc;
}

//******************************************************************************

// Author: Dylan P
unsigned int directory::stringToChars(string src, char *dst) {
    unsigned rc = src.length() < 32;
    
    if (rc) {
        for (unsigned int i = 0; i < src.length(); i++) {
            dst[i] = src[i];
        }
        
        dst[src.length()] = '\0';
    }
    
    return rc;
}

//******************************************************************************

// Author: Dylan Poirier
unsigned int directory::search(unsigned int dirIndex, string target, 
                                                      unsigned int *index) {
    unsigned int rc = 0;
    iNode *iNodePool = (iNode*) iNodes->getPool();
    unsigned int addr = iNodePool[dirIndex].addr;
    dirEntry *dir = (dirEntry*) (dataBlocks->getPool() + addr * 4096);
    unsigned int numEntries = iNodePool[dirIndex].size / sizeof(dirEntry);
    
    for (unsigned int i = 0; i < numEntries; i++) {
        dirEntry currEntry = dir[i];
        if (strcmp(currEntry.filename, target.c_str()) == 0) {
            if (index != nullptr) {
                *index = dir[i].iNodeIndex;
            }
            
            rc = 1;
            break;
        }
    }
    
    return rc;
}

//******************************************************************************

// Author: Dylan Poirier
unsigned int directory::mkdir(string path, blocks *iNodes, blocks *dataBlocks) {
    iNode *iNodePool = (iNode*) iNodes->getPool();
    string target;
    int pathIndex = resolvePath(path, target);
    if (pathIndex != -1) {
        unsigned int rc = iNodePool[pathIndex].fileType == DIR;
    
        rc = search(pathIndex, target, NULL) ? 0 : rc;
    } else {
        rc = 0;
    }
    
    if (rc && iNodes->findFree() != -1 && target.length() < 32) {
        int dirIndex = iNodes->allocate();
        
        dirEntry parentEntry = {"..", pathIndex};
        dirEntry selfEntry = {".", dirIndex};
        dirEntry entry {};
        
        iNode *parentNode = &iNodePool[pathIndex];
        char *parentBlock = dataBlocks->getPool() + (parentNode->addr * 4096);
        stringToChars(target, entry.filename);
        entry.iNodeIndex = dirIndex;
        memcpy(&parentBlock[parentNode->size], &entry, sizeof(dirEntry));

        parentNode->mTime = parentNode->cTime = time(nullptr);
        parentNode->size += sizeof(dirEntry);
        parentNode->nLinks++;
        
        iNode *dir = &iNodePool[dirIndex];
            
        iNodeInit(dir, dataBlocks);
        
        char *selfBlock = dataBlocks->getPool() + (dir->addr * 4096);
        memcpy(&selfBlock[0], &selfEntry, sizeof(dirEntry));
        memcpy(&selfBlock[sizeof(dirEntry)], &parentEntry, sizeof(dirEntry));
    }
    
    return rc;
}
