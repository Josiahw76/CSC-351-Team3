#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>

#define DEFAULT_PORT 8080

using namespace std;

int main() {
	int rc;

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;  // Receiving socket
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	if (::bind(serverSocket, (sockaddr*)&serverAddress,
         sizeof(serverAddress)) == -1) {
    	perror("bind");
    	exit(1);
	}

	if (listen(serverSocket, 5) < 0) {
		// printf("listen failed\n");
		exit(1);
	}/* else {
		printf("listen succeeded\n");
	} */
	int opt = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
           &opt, sizeof(opt));
	int clientSocket = accept(serverSocket, nullptr, nullptr);

	char buffer[1024] = {0};
	
	while (true) {
		rc = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (rc > 0) {
			buffer[rc] = '\0';
			cout << "> " << buffer << endl;
		} else {
			break;
		}
		// send(clientSocket, ack, strlen(ack), 0);
	}
	close(serverSocket);
}
