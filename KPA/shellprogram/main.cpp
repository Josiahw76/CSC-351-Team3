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


// Global Variables
#define DEFAULT_PORT 8080
#define LOCALHOST "127.0.0.1"
kpaTCP *tcpSocket;
string args = ""; // args is global as nearly every command call uses it.


using namespace std;

int main(int argc, char *argv[]) {

	// INIT PHASE

	string activeIP = LOCALHOST;
	int activePort = DEFAULT_PORT;

	// Determine whether to use default or given values
	
	tcpSocket = new kpaTCP(activeIP, activePort, false);

	// OPERATIONS PHASE

	string input;
	char retString[4096];
	char command[8];
	COMMAND opcode;
	unsigned int i = 0;
	int bytesReceived;
	bool exitFlag = false;

	while (!exitFlag) {
		// Clean variables
		memset(command, ' ', sizeof(command));
		i = 0;
		args.clear();

		// Read input until a newline
		getline(cin, input);

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
		// Null terminator to prevent reading out of bounds
		command[i] = '\0';

		opcode = strToCmd(command);
		if (opcode != UNKNOWN) {
			// Get everything after the command
			args = input.substr(strlen(command), input.size());

			// Trim whitespace from the args
			trimLR(args);
		} else {
			printf("Command \"%s\" not recognized\n",command);
			continue;
		}
		
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

			// Shutdown flows down into exit.
			case SHUTDOWN:
			shutdown();

			case EXIT:
			exitFlag = true;
			break;

			default:
			printf("Error: Invalid input\n");
		} 

		/* Debug lines for connection testing
		tcpSocket->commsWrite(input, sizeof(input));

		// Receive results of command from FS
		bytesReceived = tcpSocket->commsRead(retString, 
					sizeof(retString));
		if (bytesReceived > 0) {
			// Append message with null terminator to avoid
			// reading into bad values.
			retString[bytesReceived] = '\0';
		}
		*/
	}

	// SHUTDOWN PHASE

	delete tcpSocket;
	return 1;
}
