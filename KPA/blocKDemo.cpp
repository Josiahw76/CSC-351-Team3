#include <iostream>
#include <cstring>
#include <ctime>

//******************************************************************************

// Note, much of this material belongs in a header file

//******************************************************************************

const unsigned int blockSize = 4 * 1024;
const unsigned long long blockCount = (2LL * 1024 * 1024 *1024)/blockSize;

//******************************************************************************

// the following size makes for a dirEntry that evenly divides into blockSize
// But, it would be simple to still use the 256 fname size.  Even though
// it would not evenly divide into blockSize, the constant dirEntryPerBlock
// would work in an iterator just fine; i.e., one should be able to change
// fnameMaxSize to 256 (or any other size) and the code should all still work,
// provided the code iterates on dirEntryPerBlock
//
// This would work as the larger dataType and indexType arrays will
// set the union size to 4096 bytes; it's ok for a union member to be smaller
// than other union members; i.e., the union is sized to the largest union
// member.
const unsigned int fnameMaxSize = 32 - sizeof(unsigned int);

//******************************************************************************

typedef unsigned char dataType;
typedef unsigned int indexType;
typedef struct dirEntryType {
    unsigned int iNodeIndex;
    char fname[fnameMaxSize];
} dirEntryType;

//******************************************************************************

const unsigned int charPerBlock = blockSize / sizeof(dataType);
const unsigned int indexPerBlock = blockSize / sizeof(indexType);
const unsigned int dirEntryPerBlock = blockSize / sizeof(dirEntryType);

//******************************************************************************

typedef enum fileType {
    DIR_TYPE = 0,
    FILE_TYPE = 1
} fileType;

//******************************************************************************

// a simple iNodeType

typedef struct iNodeType {
    time_t createTime, lastAccessTime, modTime;
    unsigned int uid, gid;
    unsigned int mask;
    unsigned int fileSize;
    indexType indirectBlockIndex;
    fileType type;
} iNodeType;

//******************************************************************************

typedef union db {
    dataType data[charPerBlock];
    indexType index[indexPerBlock];
    dirEntryType dirEntry[dirEntryPerBlock];
} db;

//******************************************************************************

using namespace std;

//******************************************************************************

// define the size in bytes of the data that we will put into a file named
// Sally
const unsigned int sallySize = 26;

//******************************************************************************

int main() {
    // allocate the full array of blocks.
    db *pRaw = new db[blockCount];

    // declare some useful pointers; these are convenience pointers that are
    // derived at runtime from the various indices.
    indexType *pInd1, *pInd2;
    dirEntryType *pDirEntry;
    dataType *pData;

    if (!pRaw) {
	cout << "no go" << endl;
	exit(1);
    }

    // get a feel for what the pointer value is, printed in hex
    fprintf(stdout, "pRaw = %p\n", pRaw);

    // This will presume that at last one file or directory has been created
    // in "/", let's say it is called "Sally", and it is a file,
    // not a directory
    //
    // for this exercise, we are presuming the following:
    //    block 0 has been allocated to be this root's ind1 block
    //    block 1 has been allocated to be this root's first ind2 block
    //    block 2 has been allocated to be this root's first dirEntry block
    //              remember, iNode 0 is the iNode of the root directory
    //    block 3 has been allocated to be Sally's ind1 block
    //    block 4 has been allocated to be Sally's ind2 block
    //    block 5 has been allocated to be Sally's first data block
    //
    // create an iNode for "/"
    iNodeType iNodes[2];

    iNodes[0].createTime = time(NULL);
    iNodes[0].lastAccessTime = iNodes[0].createTime;
    iNodes[0].modTime = iNodes[0].createTime;

    iNodes[0].uid = 0;
    iNodes[0].gid = 0;

    iNodes[0].mask = 0777;
    iNodes[0].indirectBlockIndex = 0;
    iNodes[0].type = DIR_TYPE;

    // set the file size to account for the one directory entry we have
    // which is one directory entry
    iNodes[0].fileSize = 1 * sizeof(dirEntryType);


    // let's now fill in the iNode for Sally
    iNodes[1].createTime = time(NULL);
    iNodes[1].lastAccessTime = iNodes[1].createTime;
    iNodes[1].modTime = iNodes[1].createTime;

    iNodes[1].uid = 111;
    iNodes[1].gid = 111;

    iNodes[1].mask = 0644;
    iNodes[1].indirectBlockIndex = 3;
    iNodes[1].type = FILE_TYPE;

    // set the file size to account for sallySize bytes of data
    // that we'll put into it below
    iNodes[1].fileSize = sallySize;

    // *************************************************
    // set up the indices for root
    pInd1 = (indexType *)&pRaw[iNodes[0].indirectBlockIndex];
    pInd2 = (indexType *)&pRaw[1];
    pDirEntry = (dirEntryType *)&pRaw[2];

    // use the index perspective to set the index values inside ind1 and ind2
    // for this iNode
    pInd1[0] = 1;
    pInd2[0] = 2;


    // fill in the first dirEntry of root to be Sally
    pDirEntry[0].iNodeIndex = 1;
    strcpy(pDirEntry[0].fname, "Sally");
    // The root directory now has a file named Sally
    //***************************************************************

    // ****************************
    // Now, let's get some data in Sally
    // let's now get some data into the data block
    // Note, this is completely ficticious; iNode[0] is for the root directory
    // which means that it should be indexing into a dirEntryType block, not
    // a data block
    //
    // Let's get our pointers to Sally's data block, somewhat as if we
    // transversed the directory system
    //
    // somehow we found the index to root's directory entry for Sally and we
    // have a pointer to it; in fact, for this exercise, pDirEntry is still
    // pointing to it

    // get a pointer to the iNodeEntry for Sally
    iNodeType *sallyInode = &iNodes[pDirEntry->iNodeIndex];

    // get a pointer to the first-level index block for Sally
    pInd1 = (indexType *)&pRaw[sallyInode->indirectBlockIndex];
    pInd1[0] = 4;

    // get a pointer to the first second-level index block for Sally
    pInd2 = (indexType *)&pRaw[pInd1[0]];
    // set the index to refer to the first data block of Sally
    pInd2[0] = 5;

    // get a pointer to the first data block of Sally
    pData = (dataType *)&pRaw[pInd2[0]];

    for (unsigned int i = 0; i < sallySize; i++) {
	pData[i] = 65 + i;
    }

    // EVERYTHING IS NOW SET; we've mysteriously now have a file system
    // that has a single file in "/" named Sally
    //
    
    //******************************************************************
    // Let's pretend that we now go to Sally to print it's contents

    // By design, we know that the iNode for "/" is iNode[0]
    //
    // By some version of search, we have found that Sally is the
    // first entry in root's dirEntry table.

    // Get a pointer to root's iNode
    iNodeType *pInode = &iNodes[0];

    // Get a pointer to the first level indirect block
    pInd1 = (indexType *)&pRaw[pInode->indirectBlockIndex];

    // Somehow we know that Sally's dirEntry is first, so we get a
    // pointer to the first second-level index block for root
    pInd2 = (indexType *)&pRaw[pInd1[0]];

    // Somehow we know that Sally's dirEntry is first, so we get a
    // pointer to the first directroy entry in the first second-level
    // directory block
    pDirEntry = (dirEntryType *)&pRaw[pInd2[0]];

    // now, we get the index of Sally's iNode and create a pointer
    // to Sally's inode
    pInode = iNodes + pDirEntry->iNodeIndex;

    // verify that we have Sally's dir entry by printing the
    // file name
    cout << endl << "The dirEntry we are working with is for ";
    cout << pDirEntry->fname << endl << endl;

    // Get a pointer to the first level indirect block for Sally
    pInd1 = (indexType *)&pRaw[pInode->indirectBlockIndex];

    // We want the beginning of Sally, so we go to the first
    // second-level index block
    pInd2 = (indexType *)&pRaw[pInd1[0]];

    // We want the beginning of Sally, so we go to the first
    // data block of Sally
    pData = (dataType *)&pRaw[pInd2[0]];

    // Now print all the data in Sally
    for (unsigned int i = 0; i < pInode->fileSize; i++) {
	cout << pData[i];
    }

    // add a couple of end-lines to the data for the output
    cout << endl << endl;


    // ************************************************************
    // now demonstrate how the offsets work for the different types
    // of union members
    fprintf(stdout, "pRaw+1 diff = %u\n", (unsigned char*)&pRaw[1]
	    - (unsigned char *)pRaw);

    fprintf(stdout, "pRaw[5].data[10] offset is = %u\n",
	(unsigned char *)&pRaw[5].data[10] - (unsigned char *)pRaw);
    fprintf(stdout, "pRaw[5].index[10] offset is = %u\n",
	(unsigned char *)&pRaw[5].index[10] - (unsigned char *)pRaw);
    fprintf(stdout, "pRaw[5].dirEntry[10] offset is = %u\n",
	(unsigned char *)&pRaw[5].dirEntry[10] - (unsigned char *)pRaw);

    // Now let's look at it by the number of indices from the start
    // for each data type
    cout << endl;

    fprintf(stdout, "pRaw[5].data[10] offset is = %u\n",
	(dataType *)&pRaw[5].data[10] - (dataType *)pRaw);
    fprintf(stdout, "pRaw[5].index[10] offset is = %u\n",
	(indexType *)&pRaw[5].index[10] - (indexType *)pRaw);
    fprintf(stdout, "pRaw[5].dirEntry[10] offset is = %u\n",
	(dirEntryType *)&pRaw[5].dirEntry[10] - (dirEntryType *)pRaw);
}
