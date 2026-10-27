#include "fdTable.h"

// Template created by Josiah

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
	delete [] openFiles;
}

//******************************************************************************

unsigned int fdTable::add_entry(Entry file) {
	
}

//******************************************************************************

void fdTable::rm_entry(unsigned int fd) {

}

//******************************************************************************

void fdTable::openf(unsigned int address, char mode) {

}

//******************************************************************************

void fdTable::closef(unsigned int fd) {

}

//******************************************************************************


