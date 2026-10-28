#ifndef KPAFSCOMMANDS_H
#define KPAFSCOMMANDS_H
#include <string>
#include "iNode.h"

using namespace std;

// Directory entry layout from blocKDemo.cpp
const unsigned int fnameMaxSize = 32 - sizeof(unsigned int);

typedef struct dirEntryType {
    unsigned int iNodeIndex;
    char fname[fnameMaxSize];
} dirEntryType;

// Block views from blocKDemo.cpp
const unsigned int blockSize = 4 * 1024;
typedef unsigned char dataType;
typedef unsigned int indexType;

const unsigned int charPerBlock = blockSize / sizeof(dataType);
const unsigned int indexPerBlock = blockSize / sizeof(indexType);
const unsigned int dirEntryPerBlock = blockSize / sizeof(dirEntryType);

typedef union db {
    dataType data[charPerBlock];
    indexType index[indexPerBlock];
    dirEntryType dirEntry[dirEntryPerBlock];
} db;

// Search an initialized directory; return the matching inode index or -1.
int findINodeInDirectory(const iNode &directory, const db blockPool[], string name);

// Find the directory inode for the shell's absolute pwd and -1 means failure
int findDirectoryByPath(string path, const iNode iNodePool[], const db blockPool[]);

// Search entryCount valid entries, rerun the position or -1
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name);

//******************************************************************************

// Author: Simeon K
// FS operations requested by the shell.

// Check the file name arguments, pwd is sperate.
bool parseCreateArgs(string requestArgs, string &name);

// Set creation metadata for a selected iNode.
void setCreationMetadata(iNode &node, fileType type, uint16_t ownerUID, uint16_t ownerGID, uint16_t permissions);

string formatPermissions(uint16_t perms);

// These pools are defined kpaFSmain.cpp
extern iNodeType *iNodePool;
extern db *blockPool;

// Return a status byte, a space, and the pwd.
string my_chdir(string path, string pwd);

// Copy file bytes and advance position and Return the number copied
// The caller supplies enough buffer space for the bytes being read
uint32_t readFileData(iNode &node, uint32_t &position, char buffer[], uint32_t nbytes);

// Return the parent directory for the shell's cd .. request
string my_rewinddir(string pwd);


#endif
