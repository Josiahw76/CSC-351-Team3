// Author: Josiah Winiecki

#include "common.h"
#include "KPAsocket.h"

using namespace std;

void startTCP(string ip, int port) {
	tcpSocket = new kpaTCP(ip, port, true);
}

// Reception equivalent of the issueCommand function of shell.
// listen, parse, execute, evaluate, return, done.
string receiveCommand() {

	// listen
	basket char[4096];
	tcpSocket->commsListen(basket, sizeof(basket));

	// parse
	// Explicit cast to our enum type for extra safety
	SYS_CALL call = static_cast<SYS_CALL>basket[0];

	// Several things need to happen now.
	// 1.	The char buffer must be parsed further to extract all args.
	// 	1.1	This necessitates a full-blown arg parser
	// 	1.2	More variables are needed to store these, potentially
	// 		several global variables.
	// 2.	A credential check function is needed to verify vailidity
	// 	of calls.
	// 	2.1	Credentials need to be implemented as a global variable.
	// Luckily credentials can be skipped for now, until
	// we implement phase 2.
	// Parser will still extract them, but code to deal with them is
	// not needed yet. Comments will mark the correct positions to deal
	// with credential checking.
	
	// The return string takes the form of:
	// <call> <credentials> [pwd] [args]

	// For now,
	string rc = basket;

	// First, check if there are even pwd or args in the string.
	if (rc.size() > 5) {

		// The start of the string looks like:
		// call ' ' uid ':' gid ' '
		// one byte each, so begin from position 6
		args = rc.substr(6, rc.size());
	}
	// No else needed, the system knows not to read from args
	// depending on the system call made.
	
	// Now, conveniently, the pwd and args are separated by a single
	// blank, which makes them easy to separate.
	
	
}
