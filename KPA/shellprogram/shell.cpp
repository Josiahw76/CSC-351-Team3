// Author: Josiah Winiecki

#include <string>
#include "shell.h"
#include "kpaTCP.h"

using namespace std;

string issueCommand(SYS_CALL call, bool givePWD, bool useArgs) {

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
	}

	// If everything is good, send payload to FS
	if (doWrite) { 
		char *str = (char *)msg.c_str();
		tcpSocket->commsWrite(str, sizeof(str));

		// The large size is to avoid limiting the amount of data
		// that can be received from the FS process.	
		tcpSocket->commsRead(rc, 4096);

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
	// string rc = getAckArgs(issueCommand("cd", true, true));

	// If args looks like a path, call chdir
	// If args is .. , call rewinddir

	string rc;
	// Begin parsing the args if any were given
	if (!args.empty()) {

		// Check for the backtrack arg and whether backtracking
		// is possible.
		if (args.compare("..") {
			rc = issueCommand("rewinddir", true, false);
		} else {
			// Args are non-zero and don't contain backtrack flag.
			rc = issueCommand("chdir", true, true);
		}
	}
	// Whatever the FS returns is the canonical pwd.
	pwdStr = getAckArgs(rc);
}

void mkdir() {
	string rc = issueCommand("mkdir", true, true);
}

void pwd() {
	// Quick and dirty terminal print
	const char *pwd = pwdStr.c_str();
	printf("%s\n", pwd);
}

void stat() {
	string rc = issueCommand("stat", false, true);

	// Cast first byte as int and check if it indicates success.
	if ((int)rc[0] > 0) {
		
		// Data ready for parse
		string data = getAckArgs(rc);
	}
}

void touch() {
	// Q. Where are the cases handled?
	// A. FS Process will handle checking and creation, and which to do.
	string rc = issueCommand("touch", true, true);
}

// LCPIN
void lcp() {

}

// LCPOUT
void Lcp() {

}

// Tells the FS to save and shut down
void shutdown() {
	string rc = issueCommand("shutdown", false, false);
}

// Exit is handled in the switch case statement in main, 
// so no need to do anything here.

