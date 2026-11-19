// Author: Josiah Winiecki

// Many inline functions are defined within the shell.h header file.
// These include getCredentials() and trimLR().

#include "shell.h"
#include "common.h"
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
string args_raw = "";
string args[32];
string credentials;
string pwdStr;
string altArgs;

unsigned int argCount;

// Phase 1 default values
uint8_t uid = 0;
uint8_t gid = 0;

using namespace std;

int main(/*int argc, char *argv[]*/) {

	// INIT PHASE

	credentials = genCredentials();

	// This phase can be improved by adding checks for provided IP and port
	string activeIP = LOCALHOST;
	int activePort = DEFAULT_PORT;
	
	tcpSocket = new kpaTCP(activeIP, activePort, false);


	// OPERATIONS PHASE

	string input, command;
	COMMAND opcode;
	int bytesReceived;
	bool exitFlag = false;
	unsigned int i = 0;
	pwdStr = "/";

	while (!exitFlag) {

		// *************************
		// * CLEAN AND RESET PHASE *
		// *************************

		// Reset variables
		command.clear();
		i = 0;
		args_raw.clear();

		// Fetch input from the stream
		getline(cin, input);

		// *****************
		// * PARSING PHASE *
		// *****************

		// Parse the first argument
		while (input[i] != '\0' && input[i] != ' ') {
			command += input[i];
			i++;
			// Make sure the command doesn't exceed a max size.
			// (longest accepted command is 8 characters)
			if (i > 7) {
				break;	
				// Rather than take measures here,
				// opcode of UNKNOWN will catch the error.
			}
		}

		// Convert string to COMMAND code
		opcode = strToCmd(command);
		if (opcode != UNKNOWN) {
			// Get everything after the command
			args_raw = input.substr(command.length(), input.size());

			// Trim whitespace from the args_raw
			trimLR(args_raw);

			// PHASE 2: SEPARATE ARG STRINGS INTO ARRAY

			if (!args_raw.empty()) {
				size_t l_pos, i = 0;
				size_t r_pos;

				while (true) {
				    r_pos = args_raw.find_first_of(' ');
				    args[i] = args_raw.substr(l_pos, r_pos);
				    i++;

				    l_pos = r_pos + 1;

				    // Check if our index exceeds the bounds
				    if (l_pos > args_raw.size()) {
				        break;
				    }

				    // 32+ args is just absurd, but 32 is ok
				    if (i > 31) {
				        printf("Error: too many arguments\n");
					break;
				    }
				}
				argCount = i + 1; // convert to count value
			}

		} else {
			// UNKNOWN returns control to the beginning
			// of the loop.
			printf("Command \"%s\" not recognized\n",
					command.c_str());
			continue;
		}
		// *******************
		// * EXECUTION PHASE *
		// *******************
		
		printf("Recognized command %s\n", command.c_str());
		
		// Enter the appropriate function.
		// By now it should be impossible to have an invalid code.
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

			case RM:
			rm();
			break;

			case RMDIR:
			rmdir();
			break;

			case CHMOD:
			chmod();
			break;

			case CHOWN:
			chown();
			break;

			case SUGID:
			setUIDGID();
			break;

			case SUMASK:
			setUmask();
			break;

			case CP:
			cp();
			break;

			case MV:
			mv();
			break;

			case CAT:
			cat();
			break;

			case LN:
			ln();
			break;

			// Shutdown flows down into exit.
			// both cases result in termination of this program.
			case SHUTDOWN:
			shutdown();

			case EXIT:
			exitFlag = true;
			printf("Closing shell...\n");
			break;

			default:
			printf("Default case for opcode test. Wow\n");
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
