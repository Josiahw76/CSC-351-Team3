// Author: Josiah Winiecki

#include "shell.h"
#include "kpaTCP.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define DEFAULT_PORT 8080
#define LOCALHOST "127.0.0.1"

using namespace std;

int main(int argc, char *argv[]) {

	// INIT PHASE

	string activeIP = LOCALHOST;
	int activePort = DEFAULT_PORT;

	// Determine whether to use default or given values
	

	kpaTCP *tcpSocket = new kpaTCP(activeIP, activePort, false);


	/*
	// Run init with a provided IP value if one was given
	if (!initSock(argv[1] ? argv[1] : "")) {
		perror("Init failed");
		exit(1);
	}
	*/


	// OPERATIONS PHASE

	char input[128];
	char retString[4096];
	char command[8];
	COMMAND opcode;
	unsigned int i = 0;
	int bytesReceived;

	while (true) {

		// Read input until a newline
		cin.getline(input, sizeof(input));
/*
		// Parse the command.
		while (input[i] != '\0' && input[i] != ' ') {
			command[i] = input[i];
			i++;
			// Make sure the command isn't out of bounds
			// for the buffer.
			if (i > 7) {
				break;	

				// Rather than take measures here,
				// opcode of UNKNOWN will catch the error.
			}
		}

		opcode = strToCmd(command);
		// Enter the appropriate function
		switch (opcode) {
			case LS:
			ls();
			break;

			case CD:
			cd();
			break;

			case MKDIR:
			mkdir();
			break;

			case PWD:
			pwd();
			break;

			case STAT:
			stat();
			break;

			case TOUCH:
			touch();
			break;

			case LCPIN:
			lcp();
			break;

			case LCPOUT:
			Lcp();
			break;

			case SHUTDOWN:
			shutdown();
			break;

			case EXIT:
			exit();
			break;

			case "":
			break;

			default:
			printf("Error: Command not recognized.\n");
		} 
		// Prep read variables for next use
		memset(command, ' ', sizeof(command));
		i = 0;
*/
		// Receive results of command from FS
		bytesReceived = tcpSocket->commsWrite(retString, 
					sizeof(retString));
		if (bytesReceived > 0) {
			// Append message with null terminator to avoid
			// reading into bad values.
			retString[bytesReceived] = '\0';
		}
		// Return string processing
		// ...
	}

	// SHUTDOWN PHASE

	delete tcpSocket;
	return 1;
}
