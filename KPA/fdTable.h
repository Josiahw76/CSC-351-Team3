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
		void rm_entry(unsigned int fd);
	public:
		// capacity is 2 by default
		fdTable();
		~fdTable();

		void openf(unsigned int address, char mode);
		void closef(unsigned int fd);
};

#endif
