// Author: Josiah Winiecki
// Everything in here was written by me unless a comment notes otherwise.

#include <string>
#include <stdio.h>
#include "common.h"
#include "shell.h"


using namespace std;

string issueCommand(SYS_CALL call, bool givePWD, bool useArgs, bool altArgs) {

	// Receives feedback from commsRead operation
	char rc[4096];
	string rc_str;

	// Gives ability to terminate command prior to execution
	bool doWrite = true;

	// A command string will take the form:
	// <op> <credentials> [pwd] [args]

	// Static cast preserves the exact numerical value of the enum
	string msg = static_cast<char>(call);

	// Add credentials next. This can be indicated by a flag
	// This is a global variable initialized at the start of main
	msg += ' ' + credentials;

	// Check if PWD was indicated
	if (givePWD) {
		msg += ' ' + pwdStr;
	}
	// Check if we're adding args
	if (useArgs) {
		msg += ' ' + args;

	// Otherwise, if args has anything in it, check if it isn't whitespace.
	// If non-blank characters follow something that doesn't take args,
	// cancel the operation.
	} else if ((args.length() > 0) && (args.find_first_not_of(' ') != string::npos)) {
		printf("Error: Unexpected arguments\n");
		doWrite = false;

	// This is for chained system calls. The functions below use these
	// occasionally for very specific behaviors
	} else if (altArgs) {
		msg += ' ' + altArgs;
	}


	// If everything is good, send payload to FS
	if (doWrite) { 
		char *str = (char *)msg.c_str();
		tcpSocket->commsWrite(str, sizeof(str));

		// Get return message	
		tcpSocket->commsRead(rc, sizeof(rc));

		// If the code indicates an error, print a message
		// to the terminal output based on the code.
		// We get what, 127 unique error codes?
		checkCode(rc);

		// Convert it to a c string object.
		rc_str = rc;
	}
	return rc_str;
}

void ls() {
	// Invoke issueCommand with ls and give pwd to the FS
	// This needs to somehow communicate that it wants to stat
	// every file in the current directory.
	string rc = issueCommand("_LS", true, false);

	// Print the formatted contents of rc to the terminal window
	// TODO
}

void cd() {
	// If args looks like a path, call chdir
	// If args is .. , call rewinddir

	string rc;
	// Begin parsing the args if any were given
	if (!args.empty()) {

		// Check for the backtrack arg and whether backtracking
		// is possible.
		if (args.compare("..") {
			rc = issueCommand(_REWINDDIR, true, false);
		} else {
			// Args are non-zero and don't contain backtrack flag.
			rc = issueCommand(_CHDIR, true, true);
		}
	}
	// Whatever the FS returns is the canonical pwd.
	pwdStr = getAckArgs(rc);
}

void mkdir() {
	string rc = issueCommand(_MKDIR, true, true);
}

void pwd() {
	// Quick and dirty terminal print
	char *pwd = pwdStr.c_str();
	printf("%s\n", pwd);
}

void stat() {
	string rc = issueCommand(_STAT, false, true);

	// Cast first byte as int and check if it indicates success.
	if ((int)rc[0] > 0) {
		
		// Data ready for parse
		string data = getAckArgs(rc);
	}
	// Now we would print to terminal window
	
}

void touch() {
	// Q. Where are the cases handled?
	// A. Within this function using a series of system calls.

	string rc;

	// If the file exists, we want to update the access time fields
	// If not, we want to create it.

	// Pre-emptively acquire the first byte to write back.
	altArgs = "args that mean get first byte";
	rc = issueCommand(_READ, true, false, true);

	// If this fails, the file doesn't exist
	if (rc[0] > 0) {
		// Now put the byte back into the first position.
		altArgs = getAckArgs(rc) + "num that means 0th position";
		rc = issueCommand(_WRITE, true, false, true);

		altArgs.clear();
	} else {
		// I think a file name is sufficient for this sys call.
		// Create is not mkdir, so yes.
		string rc = issueCommand(_CREAT, true, true);
	}
}

// Tells the FS to save and shut down
void shutdown() {
	string rc = issueCommand(_KILL, false, false);
}

//******************************************************************************
/// File read/write functions. These are next on my list.

// LCPIN
void lcp() {
	// Gotta parse the args here...
	// <source> <dest>
	size_t pos = args.find_first_of(' ');
	string fname = args.substr(0, pos);
	FILE *file = fopen(fname.c_str(), "r");
	if (file) {
		// Make sure it's reading from the beginning of the file
		rewind(file);

		// Get the file size
		unsigned int size = ftell(file);
		char buffer[size];

		// Dump the file into the buffer
		fread(buffer, 1, size, file);

		// Now touch filename to make sure it exists
		// First, args must be modified to only include filename.
		args = args.substr(pos + 1, string::npos);
		issueCommand(_TOUCH, true, true);

		// Add the file contents to args
		args += buffer;
		issueCommand(_WRITE, true, true);

		fclose(file);
	}
}

// LCPOUT
void Lcp() {
	// Get the separator
	size_t pos = args.find_first_of(' ');
	// Get the host filename
	string fname = args.substr(pos + 1, string::npos);
	// Open host file
	FILE *file = fopen(fname.c_str(), "w");

	// Prep the args for read operation
	// fd, buffer, nbytes

	// Get filename of FS file
	args = args.substr(0, pos);

	// We can't do buffer here and we don't know how many
	// bytes to put for nbytes, so default value there is the 
	// entire length of the file, meaning all bytes
	string rc = issueCommand(_READ, true, true);
	if (!rc[0] < 0) {
		// Write rc.size() number of 1 byte items from rc to file.
		fwrite(rc.c_str(), 1, rc.size(), file);
	} else {
		checkCode(rc);	
	}	
	fclose(file);
}



// Exit is handled in the switch case statement in main, 
// so no need to do anything here.

