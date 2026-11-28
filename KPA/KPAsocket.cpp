// Author: Josiah Winiecki
// Everything in here was written by me unless a comment notes otherwise.

#include "common.h"
#include "KPAsocket.h"
#include "kpaFScommands.h"

using namespace std;

void startTCP(string ip, int port) {
	tcpSocket = new kpaTCP(ip, port, true);
}

// Reception equivalent of the issueCommand function of shell.
// listen, parse, execute, evaluate, return, done.
string receiveCommand() {

	// Read bytes from the connected shell.
	char basket[4096];
	int bytesRead = tcpSocket->commsRead(basket, sizeof(basket));

	if (bytesRead <= 0) { // Stop on disconnect or read failure.
		return "";
	}

	// parse
	// Explicit cast to our enum type for extra safety
	SYS_CALL call = static_cast<SYS_CALL>(basket[0]); // Read the call code from the first byte.

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

	// The request string takes the form of:
	// <call> <credentials> [pwd] [args]
	string request(basket, bytesRead); // Use exactly the bytes read.
	string args;
	size_t end = request.find(' ', 2); // uid:gid can be more than three characters.
	if (end != string::npos) {
		args = request.substr(end + 1);
	}
	string pwd = extractPath(args);

	// Keep the failure code for any call that isn't connected yet.
	string rc(1, -1);
	rc += ' ';

	switch (call) {
		case _CREAT:
		// This needs a filename, mode (file or dir), and path.
		// called by Touch
		// TODO: Call my_creat when creation is implemented.
		break;

		case _OPEN:
		// Used by touch
		// TODO: Wire open to the filesystem implementation.
		break;

		case _READ:
		// touch uses this to get a byte to write back to the file
		break;

		case _WRITE:
		// touch uses this to trigger modified time.
		break;

		case _STAT: {
		// Called by stat and many other shell functions
		int addr = -1;
		string name = pwd;
		if (!pwd.empty() && pwd[0] == '/') {
			// A trailing slash can only name a directory, including root.
			if (pwd.back() == '/') {
				addr = findDirectoryByPath(pwd);
				size_t last = pwd.find_last_not_of('/');
				if (last == string::npos) {
					name = "/";
				} else {
					name = pwd.substr(0, last + 1);
					name = name.substr(name.find_last_of('/') + 1);
				}
			} else {
				// Look up the last name in its parent. It can be a file or directory.
				size_t slash = pwd.find_last_of('/');
				name = pwd.substr(slash + 1);
				string parentPath = slash == 0 ? "/" : pwd.substr(0, slash);
				int parent = findDirectoryByPath(parentPath);
				if (parent != -1) {
					addr = findINodeInDirectory(parent, name);
				}
			}
		}
		rc[0] = -3; // Invalid path
		if (addr != -1) {
			rc[0] = iNodePool[addr].type == DIR ? 2 : 1;
			rc += my_stat(addr, name);
		}
		break;
		}

		case _MKDIR:
		// Called by mkdir
		break;

		case _RM:
		// Called by RM
		break;

		case _RMDIR:
		// Called by rmdir. recursive delete
		break;

		case _CHMOD:
		case _CHOWN:
		case _LINK:
		break;

		case _KILL:
		// Save the entire FS and break the while loop in main.
		// The save operation still needs to be connected.
		break;

		default:
		printf("Error: bad read of SYS_CALL from string\n");
		break;
	}
	// Once the system call has been executed, it's time to return the
	// string.

	// Convert to char array and send over the wire
	if (tcpSocket->commsWrite(rc.data(), rc.size()) != static_cast<int>(rc.size())) {
		return "";
	}
	return rc;
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
