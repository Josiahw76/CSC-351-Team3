#include <iostream>
#include <cstring>
#include <filesystem>
#include <cerrno>
#include <unistd.h>

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>


#include "kpa.h"
#include "kpaFS.h"
#include "kpaMisc.h"
#include "kpaFSmgmt.h"
#include "kpaFScommands.h"

using namespace std;

//******************************************************************************

// GLOBAL VARIABLES:
bitmap *iNodeBitmap;
bitmap *blockBitmap;

iNodeType *iNodePool;
db *blockPool;

fdType *fds;

unsigned int rootInodeIndex;

int FSerrno;

char FSfileName[256];


unsigned int userUID;
unsigned int userGID;
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

    char c;

    char usage[] =
	    "usage: %s -f fname [-c -i n-iNodes -b K-blkcount] [-d #]\n";

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
                    fprintf(stderr, "bad -i option\n");
                    fprintf(stderr, usage, argv[0]);
                    exit(-1);
                }
                break;

            case 'f':
		// file name
		fflag = true;
		strcpy(FSfileName, optarg);
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

    if (cflag) {
	initFS(iNodeCount, kBlocks);
    } else {
	restoreFS();
    }

    // Create the file descriptor bitmap and initialize the file descriptor table
    fds = new fdType[fdCount];
    for (unsigned int fd = 0; fd < fdCount; fd++) {
	fds[fd].active = false;
    }

    // determine if this is a server or standalone implementation
    doWork(portNumber);

    // need to save the FS
    saveFS();

    return 0;
}
