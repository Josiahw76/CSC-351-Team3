#ifndef __KPATCP_H
#define __KPATCP_H

#include <string>

class kpaTCP {
    private:
	int fd;
	int clientSocket;
	int serverPortNumber;

    public:
	kpaTCP(std::string serverIPaddress, int serverPort, bool serverFlag);
	~kpaTCP();

	void commsListen();
	int commsRead(char *msgBuf, unsigned int msgLength);
	int commsWrite(char *msgBuf, unsigned int msgLength);
};

#endif
