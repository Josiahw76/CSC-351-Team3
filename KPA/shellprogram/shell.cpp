// Author: Josiah Winiecki

#include <string>
#include "shell.h"
#include "kpaTCP.h"

using namespace std;

extern kpaTCP *tcpSocket;

char *issueCommand(const char *operation, bool givePWD, bool useArgs) {

	// Receives feedback from commsRead operation
	char rc[4096];

	// Gives ability to terminate command prior to execution
	bool doWrite = true;

	// A command string will take the form op <pwd> <args>
	string msg = operation;
	if (givePWD) {
		msg.append(pwdString);
	}
	// Check if we're adding args
	if (useArgs) {
		msg.append(args);

	// If args has anything in it, check if it isn't whitespace.
	// If non-blank characters follow something that doesn't take args,
	// don't continue with operation.
	} else if ((args.length() > 0) && (args.find_first_not_of(' ') != string::npos)) {
		printf("Error: Unexpected arguments\n");
		doWrite = false;
	}

	// If everything is good, send payload to FS
	if (doWrite) { 
		char *str = (char *)msg.c_str();
		tcpSocket->commsWrite(str, sizeof(str));

		// The goal of 4096 is not to limit the amount of data
		// that can be received from the FS process.	
		tcpSocket->commsRead(rc, 4096);
	}
	return rc;
}

void ls() {
	// Invoke issueCommand with ls and give pwd to the FS
	string rc = issueCommand("ls", true, false);

	// Print the formatted contents of rc to the terminal window
	// ...
}

void cd() {
	// Begin parsing the args if any were given
	if (!args.empty()) {

		// Check for the backtrack arg and whether backtracking
		// is possible.
		if (args.compare("..") && pwdString.length() <= 1) {
			args.erase(args.find_last_of('/'), args.length());
		} else {
			// Args are non-zero and don't contain backtrack flag.
			string rc = issueCommand("cd", true, true);

			// Now to interpret the string returned by the FS

			bool greenlight;
			// greenlight = checkCode(rc);

			if (greenlight) {
				pwdString.append(args);
			}
		}
	}
}

void mkdir() {
	string rc = issueCommand("mkdir", true, true);
}

void pwd() {
	// Quick and dirty terminal print
	const char *pwd = pwdString.c_str();
	printf("%s\n", pwd);
}

void stat() {
	string rc = issueCommand("stat", false, true);
}

void touch() {
	// Where are the cases handled?
	// Need a system call for "does this file exist?"
	// Could be the chdir call expanded to do generic file checking.
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

