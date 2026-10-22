// Author: Josiah Winiecki

#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

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

// The present working directory of the Shell
// char *pwdString;

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

// Initializes the client socket used to connect to the File System.
// int initSock(const char *IP);

// Helper function that handles communications between the shell and FS.
//
// operation is what to do, givePWD = true will provide pwdString to FS,
// and args are optional arguments that were given to the shell by the user.
// some commands don't have args so the parameter is optional
char * issueCommand(const char *operation, bool givePWD, char *args = NULL);

/*	temporarily comment these out for compiler messages

// Lists the contents of the current working directory within the FS
//
// Makes a request for the contents of the directory that pwdString points to.
// The contents are placed on the output stream
void ls();

cd();

// Creates a new directory within the current working directory in the FS
mkdir();

// Gives the current working directory
void pwd();

// Returns metadata about a given file.
stat();

// Creates a file using the given filename
touch();

// Copies a file from the host OS into the FS
// Also known as LCPIN
lcp();

// Copies a file out of the FS into the host OS
// Also known as LCPOUT
Lcp();

// Terminates the File System process and closes the Shell.
shutdown();

// Detaches from the FS and terminates the Shell process
exit();

*/
