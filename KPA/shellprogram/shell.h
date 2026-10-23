// Author: Josiah Winiecki

#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include "kpaTCP.h"

using namespace std;

//******************************************************************************
// 				DEFINITIONS
//******************************************************************************

// Operation codes for switch case statement in the main.
enum COMMAND {
	LS,
	CD,
	MKDIR,
	PWD,
	STAT,
	TOUCH,
	LCPIN,
	LCPOUT,
	SHUTDOWN,
	EXIT,
	UNKNOWN
};

// Present working directory as string
string pwdString;

extern string args;

//******************************************************************************
// 				FUNCTIONS
//******************************************************************************

// Changes a string containing an operation code to the corresponding enum
// code.
inline COMMAND strToCmd(char *str) {
	COMMAND rc;
	if (strcmp(str, "ls")) {
		rc = LS;
	} else if (strcmp(str, "cd")) {
		rc = CD;
	} else if (strcmp(str, "mkdir")) {
		rc = MKDIR;
	} else if (strcmp(str, "pwd")) {
		rc = PWD;
	} else if (strcmp(str, "stat")) {
		rc = STAT;
	} else if (strcmp(str, "touch")) {
		rc = TOUCH;
	} else if (strcmp(str, "lcp")) {
		rc = LCPIN;
	} else if (strcmp(str, "Lcp")) {
		rc = LCPOUT;
	} else if (strcmp(str, "shutdown")) {
		rc = SHUTDOWN;
	} else if (strcmp(str, "exit")) {
		rc = EXIT;
	} else {
		rc = UNKNOWN;
	}
	return rc;
}

// Used to maintain consistent format with args.
// Does not fix issues inside the string itself.
inline void trimLR(string &str) {
	while (str[0] == ' ' && str.length() > 0) {
		str.erase(0, 1);
	}
	while (str.back() == ' ' && str.length() > 0) {
		str.erase(str.length() - 1, 1);
	}
}

// Helper function that handles communications between the shell and FS.
//
// operation is what to do, givePWD = true will provide pwdString to FS,
// and args are optional arguments that were given to the shell by the user.
// some commands don't have args so the parameter is optional.
//
// Handles message reception as well.
char *issueCommand(const char *operation, bool givePWD, bool useArgs = true);

// Lists the contents of the current working directory within the FS
//
// Makes a request for the contents of the directory that pwdString points to.
// The contents are placed on the output stream
void ls();

// Changes the present working directory.
//
// Provides to the FS pwdString and args.
void cd();

// Creates a new directory within the current working directory in the FS
void mkdir();

// Gives the current working directory
// Does not communicate with FS: just prints pwdString
void pwd();

// Returns metadata about a given file.
void stat();

// Creates a file using the given filename if it does not exist.
// Otherwise updates modified and access times.
void touch();

// Copies a file from the host OS into the FS
// Also known as LCPIN
void lcp();

// Copies a file out of the FS into the host OS
// Also known as LCPOUT
void Lcp();

// Terminates the File System process and closes the Shell.
void shutdown();

