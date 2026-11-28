#include <iostream>
#include <climits>
#include <new>

#include <cstdio>
#include <unistd.h>

#include "KPAsocket.h"
#include "kpaFScommands.h"

using namespace std;

//******************************************************************************

// GLOBAL VARIABLES:
iNodeType *iNodePool;
db *blockPool;

// fdTable initializes its own descriptor entries.
fdTable fds;

unsigned int rootInodeIndex;

int FSerrno;

const char *FSfileName;


uint8_t uid, gid;
kpaTCP *tcpSocket;
unsigned int userMask;

unsigned int debugLevel;

//******************************************************************************

int main(int argc, char* argv[]) {
    bool cflag = false;
    bool iflag = false;
    bool bflag = false;
    bool fflag = false;
    bool err = false;

    unsigned int iNodeCount;
    unsigned int kBlocks;
    int portNumber = 8080;

    int c; // getopt returns -1 after the last option.

    char usage[] =
	    "usage: %s -f fname [-c -i n-iNodes -b K-blkcount] [-d #] [-p port]\n";

    debugLevel = 0;

    while ((c = getopt(argc, argv, "cd:i:b:f:p:s")) != -1) {
        switch(c) {
            case 'c':
		// create a new file
		cflag = true;
                break;

            case 'd':
                if (sscanf(optarg, "%u", &debugLevel) != 1) {
                    fprintf(stderr, "bad -d option\n");
                    fprintf(stderr, usage, argv[0]);
                    exit(-1);
		}
                break;

            case 'i':
		// iNode count
		iflag = true;
                if (sscanf(optarg, "%u", &iNodeCount) != 1) {
                    fprintf(stderr, "bad -i option\n");
                    fprintf(stderr, usage, argv[0]);
                    exit(-1);
                }
                break;

            case 'b':
		// block count in K
		bflag = true;
                if (sscanf(optarg, "%u", &kBlocks) != 1) {
                    fprintf(stderr, "bad -b option\n");
                    fprintf(stderr, usage, argv[0]);
                    exit(-1);
                }
                break;

            case 'f':
		// file name
		fflag = true;
		FSfileName = optarg;
                break;

            case 'p':
                if (sscanf(optarg, "%d", &portNumber) != 1) {
                    fprintf(stderr, "bad -p option\n");
                    fprintf(stderr, usage, argv[0]);
                    exit(-1);
                }
                break;

            case '?':
                err = true;
	}
    }

    if (err) {
        fprintf(stderr, "bad option given\n");
        fprintf(stderr, usage, argv[0]);
        exit(-1);
    }

    if (!fflag) {
        fprintf(stderr, "%s: missing -f option\n", argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(-1);
    }

    if ((cflag && (!iflag || !bflag)) || (!cflag && (iflag || bflag))) {
        fprintf(stderr, "%s: -c, -i, or -b used without the others\n", argv[0]);

        fprintf(stderr, usage, argv[0]);
        exit(-1);
    }

    if (debugLevel) {
	cout << "Running with debug level " << debugLevel << endl;
    }

    if (!cflag) {
        cerr << "Restoring " << FSfileName << " is not implemented yet." << endl;
        return 1;
    }

    // -b is in units of 1024 blocks. blocks stores its byte counts in unsigned int.
    if (iNodeCount == 0 || iNodeCount > UINT_MAX / sizeof(iNodeType) ||
        kBlocks == 0 || kBlocks > UINT_MAX / blockSize / 1024) {
        cerr << "Invalid inode count or block count." << endl;
        return 1;
    }
    unsigned int blockCount = kBlocks * 1024;
    blocks inodeBlocks(iNodeCount, sizeof(iNodeType));
    blocks dataBlocks(blockCount, blockSize);

    // The pools own the memory. Start all inode generations at zero.
    iNodePool = new (inodeBlocks.getPool()) iNodeType[iNodeCount]{};
    blockPool = new (dataBlocks.getPool()) db[blockCount];

    // On a new filesystem the first allocated inode is root, inode 0.
    rootInodeIndex = inodeBlocks.allocate();
    iNode &root = iNodePool[rootInodeIndex];
    setCreationMetadata(root, DIR, 0, 0, 0777);

    // Link the two index blocks to the block holding root's directory entries.
    root.addr = dataBlocks.allocate();
    unsigned int second = dataBlocks.allocate();
    unsigned int data = dataBlocks.allocate();
    blockPool[root.addr].index[0] = second;
    blockPool[second].index[0] = data;
    blockPool[data].dirEntry[0] = {rootInodeIndex, "."};
    blockPool[data].dirEntry[1] = {rootInodeIndex, ".."};
    root.size = 2 * sizeof(dirEntryType);

    // Listen for the next shell when the current shell exits.
    cerr << "Running in memory. Saving " << FSfileName << " is not implemented yet." << endl;
    startTCP("127.0.0.1", portNumber);
    while (true) {
        tcpSocket->commsListen();
        while (!receiveCommand().empty()) {
            // Keep reading commands from the connected shell.
        }
    }

    // need to save the FS
}
