// Author: Josiah Winiecki

#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

enum COMMAND {
	LS,
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

inline COMMAND strToCmd(char *str) {
	COMMAND rc;
	if (strcmp(str, "ls")) {
		rc = LS;
	} else if (strcmp(str, "mkdir") {
		rc = MKDIR;
	} else if (strcmp(str, "pwd") {
		rc = PWD;
	} else if (strcmp(str, "stat") {
		rc = STAT;
	} else if (strcmp(str, "touch") {
		rc = TOUCH;
	} else if (strcmp(str, "lcp") {
		rc = LCPIN;
	} else if (strcmp(str, "Lcp") {
		rc = LCPOUT;
	} else if (strcmp(str, "shutdown") {
		rc = SHUTDOWN;
	} else if (strcmp(str, "exit") {
		rc = EXIT;
	} else {
		rc = UNKNOWN;
	}
	return rc;
}

// 
// send(clientSocket, buffer, strlen(buffer), 0);


// The present working directory of the Shell
char *pwdString;

// socket id/adresses for use by issueCommand and init
int clientSocket;
sockaddr_in serverAddress{};


// Initializes the client socket used to connect to the File System.
initSock(const char *IP);

// Helper function that handles communications between the shell and FS.
//
// operation is what to do, givePWD = true will provide pwdString to FS,
// and args are optional arguments that were given to the shell by the user.
// some commands don't have args so the parameter is optional
char * issueCommand(const char *operation, bool givePWD, char *args = NULL);

// Lists the contents of the current working directory within the FS
//
// Makes a request for the contents of the directory that pwdString points to.
// The contents are placed on the output stream
void ls();

// Creates a new directory within the current working directory in the FS
mkdir();

// Gives the current working directory
pwd();

// Returns metadata about a given file.
stat();

// Creates a file using the given filename
touch();

// Copies a file from the host OS into the FS
lcp();

// Copies a file out of the FS into the host OS
Lcp();

// Terminates the File System process and closes the Shell.
shutdown();

// Detaches from the FS and terminates the Shell process
exit();
