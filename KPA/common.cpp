// Author: Josiah W

#include "common.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h> // For inet_ntoa

// Common definitions for shell and FS
// Things like SYS_CALL enumerations and formatting helper functions, as well
// as the kpaTCP class.
// 
// Simply include common.h to use

using namespace std;

//*****************************************************************************
// 				GENERIC FUNCTIONS
//*****************************************************************************

COMMAND strToCmd(string str) {
	COMMAND rc;
	if (str == "ls") {
		rc = LS;
	} else if (str == "cd") {
		rc = CD;
	} else if (str == "mkdir") {
		rc = MKDIR;
	} else if (str == "pwd") {
		rc = PWD;
	} else if (str == "stat") {
		rc = STAT;
	} else if (str == "touch") {
		rc = TOUCH;
	} else if (str == "lcp") {
		rc = LCPIN;
	} else if (str == "Lcp") {
		rc = LCPOUT;
	} else if (str == "shutdown") {
		rc = SHUTDOWN;
	} else if (str == "exit") {
		rc = EXIT;
	} else {
		rc = UNKNOWN;
	}
	return rc;
}

string getAckArgs(string ack) {
	// We're expecting an exact format for the string
	// A byte for return code, a space, then any content

	// Return everything beyond the first two bytes of the string.
	return ack.substr(2);
}

void checkCode(string rc) {
	// Interpret the first byte as an int and 
	switch ((int)rc[0]) {
		case 1:
		// This is the default success code so simply break.
		break;

		case 0:
		case -1:
		printf("Generic failure\n");
		break;

		// In any case where an operation failed due to lack of
		// permissions, this error code shall be used.
		case -2:
		printf("Permission denied.\n");
		break;

		// The given path was invalid somehow.
		case -3:
		printf("Error: invalid path");
		break;

		// Error code is not something we expected
		default:
			printf("Unknown error\n");
	}

}

void trimLR(string &str) {
	while (str[0] == ' ' && str.length() > 0) {
		str.erase(0, 1);
	}
	while (str.back() == ' ' && str.length() > 0) {
		str.erase(str.length() - 1, 1);
	}
}

string genCredentials() {
	// Return UID:GID. These are global variables.
	return to_string(uid) + ":" + to_string(gid);
}

//*****************************************************************************
// 				KPATCP FUNCTIONS
//*****************************************************************************

// I (Josiah) did not write these, they were provided in the kpaTCP source.

kpaTCP::kpaTCP(string serverIPaddress, int serverPort, bool serverFlag) {
    serverPortNumber = serverPort;

    clientSocket = fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
	cerr << "Error creating socket" << endl;
	exit(-1);
    }

    // Bind socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPortNumber);

    if (serverFlag) {
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	if (::bind(fd,
		(struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
	    cerr << "Error binding socket" << endl;
	    close(fd);
	    exit(-1);
	}
    } else {
	serverAddress.sin_addr.s_addr = inet_addr(serverIPaddress.c_str());
	if (connect(clientSocket, (struct sockaddr*)&serverAddress,
		sizeof(serverAddress)) < 0) {

	    std::cerr << "Error connecting to server" << std::endl;
	    close(clientSocket);
	    exit(-1);
	}
    }
}

//******************************************************************************

kpaTCP::~kpaTCP() {
    close(clientSocket);
    close(fd);
}

//******************************************************************************

void kpaTCP::commsListen() {
    if (listen(fd, 1) < 0) { // only allow one connection
        cerr << "Error listening on socket" << endl;
        close(fd);
    } else {
	cout << "FS listening on port " << serverPortNumber << endl;
    }

    // wait for a client
    sockaddr_in clientAddress;
    socklen_t clientAdressLength = sizeof(clientAddress);
    clientSocket = accept(fd, (struct sockaddr*)&clientAddress,
	    &clientAdressLength);

    if (clientSocket < 0) {
        cerr << "Error accepting client connection" << endl;
        close(fd);
	exit(-1);
    }


    cout << "Client connected from " << inet_ntoa(clientAddress.sin_addr)
	    << ":" << ntohs(clientAddress.sin_port) << endl;
}

//******************************************************************************

int kpaTCP::commsRead(char *msgBuf, unsigned int msgLength) {
    return read(clientSocket, msgBuf, msgLength);
}

//******************************************************************************

int kpaTCP::commsWrite(char *msgBuf, unsigned int msgLength) {
    return send(clientSocket, msgBuf, msgLength, 0);
}
