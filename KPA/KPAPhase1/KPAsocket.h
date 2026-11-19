#ifndef __KPASOCKET_H
#define __KPASOCKET_H

// Author: Josiah Winiecki

#include "shellprogram/kpaTCP.h"
#include <string>

// This suite of functions handles incoming communications between the Shell
// and the FS, and executes the appropriate system calls.


//*****************************************************************************
// 				DEFINITIONS
//*****************************************************************************

// Stores arguments provided by shell in the receiveCommand function.
string args;
string pwd;

//*****************************************************************************
// 				FUNCTIONS
//*****************************************************************************

// Initiate the TCP port with whatever values were given
void startTCP(string ip, int port);

string receiveCommand();

string extractPath(string str);

#endif
