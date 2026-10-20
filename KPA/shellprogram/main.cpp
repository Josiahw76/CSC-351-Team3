// Author: Josiah Winiecki

#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define DEFAULT_PORT 8080

using namespace std;

int clientSocket;
sockaddr_in serverAddress{};


// Setup function to build the 
int init(const char *IP) {
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


int main(int argc, char *argv[]) {

	// Run init with a provided IP value if one was given
	if (!init(argv[1] ? argv[1] : "")) {
		perror("Init failed");
		exit(1);
	}
	
	while (true) {
		char buffer[128];

		// Read input until a newline
		cin.getline(buffer, sizeof(buffer));

		// Transmit the data to the receiver
     		send(clientSocket, buffer, strlen(buffer), 0);

		/* rc = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (rc > 0) {
			cout << "> " << buffer << endl;
		} */
		memset(buffer, ' ', sizeof(buffer));

	}

	close(clientSocket);

	return 1;
}
