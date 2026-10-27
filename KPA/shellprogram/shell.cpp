// Author: Josiah Winiecki
// Everything in here was written by me unless a comment notes otherwise.

#include <string>
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
	// This needs to somehow communicated that it wants to stat
	// every file in the current directory.
	string rc = issueCommand("", true, false);

	// Print the formatted contents of rc to the terminal window
	// ...
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
	// Quick and dirty
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

	// If the file can be opened, then it exists and we can
	// write a single byte to affect the time fields.
	rc = issueCommand(_OPEN, true, true);

	if (rc[0] > 0) {
		// This indicates a success code and lets us continue
		// with rewriting a byte.
		altArgs = "args that mean get first byte";
		rc = issueCommand(_READ, true, false, true);

		// Now put the byte back into the first position.
		altArgs = getAckArgs(rc) + "num that means 0th position";
		rc = issueCommand(_WRITE, true, false, true);

		altArgs.clear();
	} else {
		// I think a file name is sufficient for this sys call.
		string rc = issueCommand(_CREAT, true, true);

		// Note that create simply won't do anything if the file exists.
	}
}

// Tells the FS to save and shut down
void shutdown() {
	string rc = issueCommand(_KILL, false, false);
}

//******************************************************************************
/// Fstream functions. These are next on my list.

// LCPIN
void lcp() {

}

// LCPOUT
void Lcp() {

}


// Exit is handled in the switch case statement in main, 
// so no need to do anything here.

