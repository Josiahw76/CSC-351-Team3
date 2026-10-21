// Author: Josiah Winiecki

#include "shell.h"

#define DEFAULT_PORT 8080

using namespace std;

int main(int argc, char *argv[]) {

	// INIT PHASE

	// Run init with a provided IP value if one was given
	if (!init(argv[1] ? argv[1] : "")) {
		perror("Init failed");
		exit(1);
	}
	
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

		// Parse the command.
		while ((input[i] != '\0' && input[i] != ' ') {
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

		// Receive results of command from FS
		bytesReceived = recv(clientSocket, retString, 
					sizeof(retString), 0);
		if (bytesReceived > 0) {
			// Append message with null terminator to avoid
			// reading into bad values.
			retString[bytesReceived] = '\0';
		}
		// Return string processing
		// ...
	}

	// SHUTDOWN PHASE

	close(clientSocket);
	return 1;
}
