// Author: Josiah Winiecki
// Everything in here was written by me unless a comment notes otherwise.

#include <string>
#include <stdio.h>
#include "common.h"
#include "shell.h"


using namespace std;

//******************************************************************************

string issueCommand(SYS_CALL call, string pwd, bool useArgs, bool useAlts) {

	// Receives feedback from commsRead operation
	char rc[4096];
	string rc_str;

	// Gives ability to terminate command prior to execution
	bool doWrite = true;

	// A command string will take the form:
	// <op> <credentials> [pwd] [args_raw]

	// Static cast preserves the exact numerical value of the enum
	string msg = " ";
	msg[0] = static_cast<char>(call);

	// Add credentials next. This can be indicated by a flag
	// This is a global variable initialized at the start of main
	msg += ' ' + credentials;

	// pwd is given to the command now, so include it here.
	// If it's supposed to be blank, the caller handles that.
	msg += pwd;

	// Check if we're adding raw args
	if (useArgs) {
		msg += ' ' + args_raw;

	// Otherwise, if args_raw has anything in it, check if it isn't whitespace.
	// If non-blank characters follow something that doesn't take arguments,
	// cancel the operation.
	} else if ((argCount > 0) && (args_raw.find_first_not_of(' ') != string::npos)) {
		printf("Error: Unexpected arguments\n");
		doWrite = false;

	// This is for chained system calls. The functions below use these
	// occasionally for very specific behaviors
	} else if (useAlts) {
		msg += ' ' + altArgs;
	}

	// ***********************************************
	// ************* Send data to FS *****************
	// ***********************************************

	// If everything is good, send payload to FS
	if (doWrite) { 
		char *str = (char *)msg.c_str();
		tcpSocket->commsWrite(str, sizeof(str));

		// Get return message	
		tcpSocket->commsRead(rc, sizeof(rc));

		// If the code indicates an error, print a message
		// to the terminal output based on the code.
		// We get what, 127 unique error codes?
		checkCode(rc);

		// Convert it to a c string object.
		rc_str = rc;
	}
	return rc_str;
}

//******************************************************************************

// dumpArg tells the function to put the target file in altArgs instead of
// leaving it on the end of the absolute path

string prepPath(string filename, bool dumpArg) {
	string path;
	trimLR(filename);
	
	// Check for '/' characters. One at the beginning indicates
	// absolute path, one somewhere in the middle denotes relative path,
	// none indicates a filename.
	size_t bs_pos = filename.find_first_of('/');

	// Get path ready
	if (bs_pos != 0) {
		// It is not an absolute path, use the one we have saved
		path = pwdStr;
		if (bs_pos != string::npos) {
			// We've determined that it is a relative path.
			// Get filename saved so we can reuse it.
			path += filename;
		}

	} else {
		// Else, bs at the front means the path is absolute
		path = filename;
	}

	if ((bs_pos != string::npos) && dumpArg) {
			bs_pos = path.find_last_of('/') + 1;

			// Grab everything from after the bs to the end of str
			filename = path.substr(bs_pos);

			// Strip the end off including the '/'
			path.erase(bs_pos - 1);

			altArgs = filename;
	}

	return path;
}

//******************************************************************************

void ls() {
	// PHASE 2 UPDATES: CHECKS FOR PATH
	string wd;
	if (argCount > 0) {
		wd = prepPath(args[0], false);
	} else {
		wd = pwdStr;
	}
	
	string rc = issueCommand(_LS, wd, false);

	// Print the formatted contents of rc to the terminal window
	// TODO
	// should be sufficient. ls should append newlines after
	// each stat.
	printf("%s\n", rc.c_str());
}

//******************************************************************************

void cd() {
	string rc;

	if (argCount == 0) {
		string wd = prepPath(args[0], false);
		rc = issueCommand(_CHDIR, wd, false, false);
		pwdStr = getAckArgs(rc);
	} else {
		printf("Error: exactly one argument allowed for cd\n");
	}
}

//******************************************************************************

void mkdir() {
	string wd;
	for (unsigned int i = 0; i < argCount; i++) {
		wd = prepPath(args[i]);
		string rc = issueCommand(_MKDIR, wd, false, true);
	}

}

//******************************************************************************

void pwd() {
	// Quick and dirty terminal print
	const char *pwd = pwdStr.c_str();
	printf("%s\n", pwd);
}

//******************************************************************************

void stat() {
	if (argCount == 1) {
		string wd = prepPath(args[0], false);
		string rc = issueCommand(_STAT, wd, false, false);

		// Cast first byte as int and check if it indicates success.
		if ((uint8_t)rc[0] > 0) {
		
			// Data ready for parse
			string data = getAckArgs(rc);

			// Print it
			printf("%s\n", data.c_str());
		}
	} else {
		printf("Error: exactly one argument allowed for cd\n");
		return;
	}

}

//******************************************************************************

void touch() {
	string wd, rc;
	// If the file exists, we want to update the access time fields
	// If not, we want to create it.

	// Pre-emptively acquire the first byte to write back.

	for (unsigned int i = 0; i < argCount; i++) {
		wd = prepPath(args[i]);
		rc = issueCommand(_READ, wd, false, true);

		// If this fails, the file doesn't exist
		if ((uint8_t)rc[0] > 0) {
			// Now put the byte back into the first position.
			altArgs = getAckArgs(rc);
			altArgs += " 0";
			rc = issueCommand(_WRITE, wd, false, true);

			altArgs.clear();
		} else {
			// I think a file name is sufficient for this sys call.
			// Create is not mkdir, so yes.
			string rc = issueCommand(_CREAT, wd, false, true);
		}
	}
}

//******************************************************************************

// Tells the FS to save and shut down
void shutdown() {
	char buf[1];
	buf[0] = _KILL;
	tcpSocket->commsWrite(buf, 1);
}

//******************************************************************************

// TODO: change send behavior so only 4KB is sent over tcp at a time.
// in other words, break up messages if file is very large

// LCPIN
void lcp() {
	// The arguments are treated as:
	// <source> <dest>
	// size_t pos = args_raw.find_first_of(' ');
	// string fname = args_raw.substr(0, pos);
	//
	// fname is effectively just args[0]
	if (argCount == 2) {

	FILE *file = fopen(args[0].c_str(), "r");
		if (file) {
			// Make sure it's reading from the beginning of the file
			rewind(file);

			// Get the file size
			unsigned int size = ftell(file);
			char *buffer = new char[size];

			// Dump the file into the buffer as last file operation
			fread(buffer, 1, size, file);
			fclose(file);
		
			// Now touch filename to make sure it exists
			// The target file should have been stored in args[1]
			args_raw = args[1];

			// Set argCount to what touch will expect.
			argCount = 1;
			touch();

			string wd = prepPath(args[1]);

			// Use altArgs for filename and contents.
			// It's used for touch but prepPath clears it.
			altArgs += ' ';
			altArgs += buffer;
	
			issueCommand(_WRITE, wd, false, true);

		} else {
			// file didn't work
			printf("Error: file could not be opened\n");
		} 
	} else {
		// argcount wasn't 2. exactly 2 are needed
		printf("Error: 2 arguments required\n");
	}
}

//******************************************************************************

// LCPOUT
void Lcp() {
	/* 
	// Get the separator
	size_t pos = args_raw.find_first_of(' ');
	// Get the host filename
	string fname = args_raw.substr(pos + 1, string::npos);
	*/
	// Everything above is just grabbing args[1]

	if (argCount == 2) {
		// Open host file
		FILE *file = fopen(args[1].c_str(), "w");

		// Prep the args_raw for read operation
		// fd, buffer, nbytes

		/*
		// Get filename of FS file
		args_raw = args_raw.substr(0, pos);
		*/
		// This above is just grabbing args[0]

		string wd = prepPath(args[0]);

		// We can't do buffer here and we don't know how many
		// bytes to put for nbytes, so default value there is the 
		// entire length of the file, meaning all bytes
		string rc = issueCommand(_READ, wd, false, true);
		if (((uint8_t)rc[0] > 0)) {
			// Lop off the return code and following blank before
			// writing to the file.
			rc.erase(0, 2);

			// Write rc.size() number of 1 byte items to file	
			fwrite(rc.c_str(), 1, rc.size(), file);
			fclose(file);
		}
	} else {
		printf("Error: 2 arguments required\n");
	}
	
}

//******************************************************************************
// PHASE 2 COMMANDS

void rm() {}

void rmdir() {}

void chmod() {}

void chown() {}

void setUIDGID() {}

void setUmask() {}

void cp() {}

void mv() {}

void cat() {}

void ln() {}

