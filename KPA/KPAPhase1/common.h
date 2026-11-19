#ifndef __COMMON_H
#define __COMMON_H

// Author: Josiah W

#include <string>
#include <stdio.h>

// Common definitions for shell and FS
// Things like SYS_CALL enumerations and formatting helper functions, as well
// as the kpaTCP class.
// 
// Simply include common.h to use

using namespace std;

//******************************************************************************
// 				DEFINITIONS
//******************************************************************************

// Forward definition
class kpaTCP;

// Global variables
extern unsigned int uid, gid;
extern string pwdStr;
extern string args;
extern kpaTCP *tcpSocket;
extern string credentials;

enum COMMAND : uint8_t {
	LS,
	CD,
	MKDIR,
	PWD,
	STAT,
	TOUCH,
	LCPIN,
	LCPOUT,
	SHUTDOWN,
	EXIT,
	UNKNOWN
};

enum SYS_CALL : uint8_t {
	_CREAT,
	_OPEN,
	_CLOSE,
	_READ,
	_WRITE,
	_LSEEK,
	_STAT,
	_FSTAT,
	_MKDIR,
	_RMDIR,
	_LINK,
	_UNLINK,
	_CHDIR,
	_OPENDIR,
	_CLOSEDIR,
	_READDIR,
	_REWINDDIR,
	_LS,
	_KILL
};

//******************************************************************************
// 			 	GENERIC FUNCTIONS
//******************************************************************************

// Converts a string into a usable COMMAND code
COMMAND strToCmd(string str);
	

// Extracts the content from an ack returned by the FS process
string getAckArgs(string ack);

// Error message printer for return codes
void checkCode(string rc);

// Used to maintain consistent format with args.
// Does not fix issues inside the string itself.
void trimLR(string &str);

// Called once to initialize the system with the given credentials
string genCredentials();

//******************************************************************************
// 			 	KPATCP CLASS
//******************************************************************************

// Author: Dr. Wheat
// This was pulled directly from the kpaTCP class

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
