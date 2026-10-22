// Author: Josiah Winiecki

#include "shell.h"

/*
int initSock(const char *IP) {
	int rc;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Optionally, set IP address
	if (strlen(IP) > 0) {
		inet_pton(AF_INET, IP, &serverAddress.sin_addr);
	}

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, 
		sizeof(serverAddress)) < 0) { 
		perror("Connection failed");
		rc = 0;
	} else {
		printf("Connection ");
		if (strlen(IP) > 1) {
			printf("to %s ",IP);
		}
		printf("successful.\n");
		rc = 1;
	}
	return rc;
}
*/

char * issueCommand(const char *operation, bool givePWD, char *args) {


}
/*		temporarily comment these out to avoid compiler messages
void ls() {
	// Invoke issueCommand with ls and give pwd to the FS
	char *rc = issueCommand("ls", true);

	// Print the formatted contents of rc to the terminal window
	// ...
}

cd() {

}

mkdir() {

}

void pwd() {
	printf("%s", pwdString);
}

stat() {}

touch() {}

lcp() {}

Lcp() {}

shutdown() {}

exit() {}
*/
