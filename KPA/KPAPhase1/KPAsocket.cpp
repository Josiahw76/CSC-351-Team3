// Author: Josiah Winiecki
// Everything in here was written by me unless a comment notes otherwise.

#include "common.h"
#include "KPAsocket.h"

using namespace std;

void startTCP(string ip, int port) {
	tcpSocket = new kpaTCP(ip, port, true);
}

// Reception equivalent of the issueCommand function of shell.
// listen, parse, execute, evaluate, return, done.
string receiveCommand() {

	// listen
	basket char[4096];
	tcpSocket->commsListen(basket, sizeof(basket));

	// parse
	// Explicit cast to our enum type for extra safety
	SYS_CALL call = static_cast<SYS_CALL>basket[0];

	// Several things need to happen now.
	// 1.	The char buffer must be parsed further to extract all args.
	// 	1.1	This necessitates a full-blown arg parser
	// 	1.2	More variables are needed to store these, potentially
	// 		several global variables.
	// 2.	A credential check function is needed to verify vailidity
	// 	of calls.
	// 	2.1	Credentials need to be implemented as a global variable.
	// Luckily credentials can be skipped for now, until
	// we implement phase 2.
	// Parser will still extract them, but code to deal with them is
	// not needed yet. Comments will mark the correct positions to deal
	// with credential checking.
	
	// The return string takes the form of:
	// <call> <credentials> [pwd] [args]

	// For now,
	string rc = basket;

	// First, check if there are even pwd or args in the string.
	if (rc.size() > 5) {

		// The start of the string looks like:
		// call ' ' uid ':' gid ' '
		// one byte each, so begin from position 6
		args = rc.substr(6, rc.size());

	}
	// No else needed, the system knows only to read from args
	// when they should be there.

	pwd = extractPath(args);

	switch (call) {

		// 

		case _CREAT:
		// This needs a filename, mode (file or dir), and path.
		// called by Touch

		rc = my_creat();
		break;

		case _OPEN:
		// Used by touch
		rc = open()
		break;

		case _READ:
		// touch uses this to get a byte to write back to the file

		rc = read();
		break;

		case _WRITE:
		// touch uses this to trigger modified time.

		rc = write();
		break;

		case _STAT:
		// Called by stat

		rc = my_stat();
		break;

		case _MKDIR:
		// Called by mkdir

		rc = mkdir();
		break;

		case _RMDIR:
		// Called by rmdir. recursive delete

		rc = rmdir();
		break;

		case _CHDIR:
		// Called by CD
	
		rc = chdir();
		break;

		case _REWINDDIR:
		// Called by CD
		rc = rewinddir();
		break;

		case _KILL:
		// Save the entire FS and break the while loop in main.
		rc = kill();
		break;

		default:
		printf("Error: bad read of SYS_CALL from string\n");
		break;
	}
	// Once the system call has been executed, it's time to return the
	// string.
	
	// Convert to char array and send over the wire
	char *buf = rc.c_str();
	commsWrite(buf, sizeof(buf));
}

string extractPath(string str) {
	string rc = "";
	size_t pos;

	if (str.size() > 0) {
		pos = str.find_first_of(' ');
		rc = str.substr(0, pos);
	}
	return rc;
}

