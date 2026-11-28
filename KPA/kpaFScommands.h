#ifndef KPAFSCOMMANDS_H
#define KPAFSCOMMANDS_H
#include <string>
#include "iNode.h"
#include "fdTable.h"
#include "blocks.h" // Provides the blocks type used by the FS call declarations.

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
int findINodeInDirectory(unsigned int directoryIndex, string name);


// Find the directory inode for the shell's absolute pwd and -1 means failure
int findDirectoryByPath(string path);

// Search entryCount valid entries, rerun the position or -1
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name);

//******************************************************************************

// Author: Simeon K
// FS operations requested by the shell.


// Set creation metadata for a selected iNode.
void setCreationMetadata(iNode &node, fileType type, uint16_t ownerUID, uint16_t ownerGID, uint16_t permissions);

string formatPermissions(uint16_t perms);

// These pools are defined kpaFSmain.cpp
extern iNodeType *iNodePool;
extern db *blockPool;

// Format inode metadata with the requested file name.
string my_stat(unsigned int addr, string name);

// Copy file bytes and advance position and Return the number copied
// The caller supplies enough buffer space for the bytes being read
uint32_t readFileData(iNode &node, uint32_t &position, char buffer[], uint32_t nbytes);

// Return bytes read, -1 for invalid input or -2 for a non readable mode
// nbytes = -1 uses the inode's size. the called provides enough buffer space
int my_read(fdTable &table, unsigned int fd, char buffer[], int nbytes = -1);

// Return 1 when closed or -1 for an invalid or unused descriptor
int my_close(fdTable &table, unsigned int fd);

struct dirStream;

// Write bytes though an open descriptor
int my_write(fdTable &table, unsigned int fd, const char buffer[], int nbytes, blocks &dataBlocks);

// Create a directory using the caller's owner ID's and permission mask.
int my_mkdir(string name, string pwd, uint16_t userUID, uint16_t userGID, uint16_t mask, blocks &inodeBlocks, blocks &dataBlocks);

// Add another name for an existing regular file.
int my_link(string oldPath, string newPath, string pwd, blocks &dataBlocks);

// Remove a file name and update its inode's link count.
int my_unlink(fdTable &table, string path, string pwd, unsigned int userUID, unsigned int userGID, blocks &inodeBlocks, blocks &dataBlocks);

// Remove a directory and its descendants.
int my_rmdir(fdTable &table, string path, string pwd, unsigned int userUID, unsigned int userGID, blocks &inodeBlocks, blocks &dataBlocks);

// Open, read, and close a directory stream.
int my_opendir(string path, string pwd, dirStream &stream);
int my_readdir(dirStream &stream, dirEntryType &entry);
int my_closedir(dirStream &stream);

// Save filesystem state and stop the request loop after saving succeeds.
int my_kill(blocks &inodeBlocks, blocks &dataBlocks, fdTable &table, string pwd, FILE *store, bool &running);
#endif