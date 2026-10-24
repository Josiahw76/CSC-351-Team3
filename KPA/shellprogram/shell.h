#ifndef __SHELL_H
#define __SHELL_H

// Author: Josiah Winiecki

#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include "common.h"

using namespace std;

//******************************************************************************
// 				DEFINITIONS
//******************************************************************************

// Global variables
extern uint8_t uid, gid;
extern string pwdStr;
extern string args;
extern kpaTCP *tcpSocket;
extern string credentials;

// Arguments provided to chained system calls by the functions below.
// These are used in things like touch to trigger access time updates.
string altArgs;

//******************************************************************************
// 				SHELL FUNCTIONS
//******************************************************************************


// Helper function that handles communications between the shell and FS.
//
// operation is what to do, givePWD = true will provide pwdStr to FS,
// and args are optional arguments that were given to the shell by the user.
// some commands don't have args so the parameter is optional.
//
// Handles message reception as well.
// TODO:	auto print error messages if the first byte is an error code.
// Maybe build in an error handler with a switch case for these?
string issueCommand(SYS_CALL call, bool givePWD, bool useArgs, bool altArgs = false);

// Lists the contents of the current working directory within the FS
//
// Makes a request for the contents of the directory that pwdStr points to.
// The contents are placed on the output stream
void ls();

// Changes the present working directory.
//
// Provides to the FS pwdStr and args.
void cd();

// Creates a new directory within the current working directory in the FS
void mkdir();

// Gives the current working directory
// Does not communicate with FS: just prints pwdStr
void pwd();

// Returns metadata about a given file.
void stat();

// Creates a file using the given filename if it does not exist.
// Otherwise updates modified and access times.
void touch();

// Copies a file from the host OS into the FS
// Also known as LCPIN
void lcp();

// Copies a file out of the FS into the host OS
// Also known as LCPOUT
void Lcp();

// Terminates the File System process and closes the Shell.
void shutdown();

#endif
