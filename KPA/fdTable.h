#ifndef __FDTABLE_H
#define __FDTABLE_H

// Template created by Josiah

struct Entry {
	unsigned int fileAddress;
	char fmode; // r = read, w = write, a = append, n = null
};

class fdTable {

	private:
		Entry *openFiles;
		unsigned int tableCapacity;
		unsigned int tableCount;

		// Returns a file descriptor
		unsigned int add_entry(Entry file);
	public:
		// capacity is 2 by default
		fdTable();
		~fdTable();

		unsigned int openf(unsigned int address, char fmode);
		void closef(unsigned int fd);
};

#endif
