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

// Search entryCount valid entries, rerun the position or -1
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name);

//******************************************************************************

// Author: Simeon K
// FS operations requested by the shell.

// Seperate the current directory from the requested filename.
bool parseCreateArgs(string requestArgs, string &directory, string &name);

// Set creation metadata for a selected iNode.
void setCreationMetadata(iNode &node, fileType type, uint16_t ownerUID, uint16_t ownerGID, uint16_t permissions);

string formatPermissions(uint16_t perms);
#endif
