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
// Author: Josiah
fdTable::~fdTable() {
	// kam, this function is one line and was already done.
	delete [] openFiles;
}

//******************************************************************************
// Author: Kam E
unsigned int fdTable::add_entry(Entry file) {
	tableInsert = 1;
	openFiles = new File[tableCount]; // <- this is very incorrect.
					 // you are trying to reassign the array
					 // to a std file object
	// So, this adds a new file to the table, with the tableCount counting it to table capacity 
	//I'm not done with this yet.
	
}

//******************************************************************************
// Author: Kam E
void fdTable::rm_entry(unsigned int fd) {
	//Remove directory: Path + FileName
	// ^^^^ no. this is "closing" the file by clearing the array entry.
	// Something I'm going to write later.
	

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


