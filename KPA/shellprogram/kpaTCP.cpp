#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h> // For inet_ntoa

#include "kpaTCP.h"

//******************************************************************************

using namespace std;

//******************************************************************************

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
