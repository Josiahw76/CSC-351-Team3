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
extern string args_raw;
extern string *args;
extern unsigned int argCount;
extern kpaTCP *tcpSocket;
extern string credentials;

// Arguments provided to chained system calls by the functions below.
// These are used in things like touch to trigger access time updates.
extern string altArgs;

//******************************************************************************
// 				SHELL FUNCTIONS
//******************************************************************************

// Helper function that handles communications between the shell and FS.
//
// operation is what to do, givePWD = true will provide pwdStr to FS,
// and args_raw are optional arguments that were given to the shell by the user.
// some commands don't have args_raw so the parameter is optional.
//
// Handles message reception as well.
string issueCommand(SYS_CALL call, string wd, bool useArgs, bool useAlts = false);

// Prepares whatever argument was given 
string prepPath(string filename, bool dumpArg = true);

// Lists the contents of the current working directory within the FS
//
// Makes a request for the contents of the directory that pwdStr points to.
// The contents are placed on the output stream
// UPDATED
void ls();

// Verifies files along the path with STAT, assigns pwdStr if valid
void cd();

// Creates a new directory within the current working directory in the FS
// UPDATED
void mkdir();

// Gives the current working directory
// Does not communicate with FS: just prints pwdStr
// UPDATED
void pwd();

// Asks the FS for metadata about a given file.
// UPDATED
void stat();

// Creates a file using the given filename if it does not exist.
// Otherwise updates modified and access times.

// UPDATED: now accepts any number of arguments
void touch();

// Copies a file from the host OS into the FS
// Also known as LCPIN

// UPDATED
void lcp();

// Copies a file out of the FS into the host OS
// Also known as LCPOUT

// UPDATED: may need revision
void Lcp();

// Terminates the File System process and closes the Shell.
// IMPROVED
void shutdown();

// PHASE 2 COMMANDS

void rm();

void rmdir();

void chmod();

void chown();

void setUIDGID();

void setUmask();

void cp();

void mv();

void cat();

void ln();



#endif
