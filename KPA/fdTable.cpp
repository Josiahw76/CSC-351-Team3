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
// Author: Kam E
fdTable::~fdTable() {
deleteFiles = Entry[tableCount];
	for (unsigned int i = 0; i < table
	deleteFiles = Entry[tableCount];
	
	delete [] openFiles;
}

//******************************************************************************
// Author: Kam E
unsigned int fdTable::add_entry(Entry file) {
	tableInsert = 1;
	openFiles = new File[tableCount];
	// So, this adds a new file to the table, with the tableCount counting it to table capacity 
	//I'm not done with this yet.
	
}

//******************************************************************************

void fdTable::rm_entry(unsigned int fd) {

}

//******************************************************************************
// Author: Kam E
unsigned int fdTable::openf(unsigned int address, char fmode) {
	unsigned int fd;

	return fd;
}

//******************************************************************************
// Author: Kam E
void fdTable::closef(unsigned int fd) {

}

//******************************************************************************


