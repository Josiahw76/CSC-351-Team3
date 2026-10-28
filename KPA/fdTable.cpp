#include "fdTable.h"

// Author: Josiah W

//******************************************************************************

fdTable::fdTable() {
	tableCapacity = 2;
	openFiles = new Entry[tableCapacity];
	for (unsigned int i = 0; i < tableCapacity; i++) {
		// set to null
		openFiles[i].fmode = 'n';
	}
}

//******************************************************************************

fdTable::~fdTable() {
	// kam, this function is one line and was already done.
	delete [] openFiles;
}

//******************************************************************************

unsigned int fdTable::add_entry(Entry file) {
	unsigned int fd;

	// Because of the way we use this table, it can literally never be
	// full.
	// Although there are two entries, we still iterate because it's
	// a scalable behavior, unlike boolean array assignment.
	// If we wanted to expand this, not much would need rewrite.

	for (unsigned int i = 0; i < tableCapacity; i++) {
		// n means null, or empty
		if (openFiles[i].fmode == 'n') {
			// Save it there and pass back the index
			openFiles[i] = file;
			fd = i;
			break;
		}
	}
	return fd;
}

//******************************************************************************

unsigned int fdTable::openf(unsigned int address, char fmode) {
	// Construct the entry
	Entry file = {address, fmode, 0};

	// pass back the fd given on insertion
	return add_entry(file);

}

//******************************************************************************

void fdTable::closef(unsigned int fd) {
	openFiles[fd].fmode = 'n';	
}

//******************************************************************************


