#ifndef __FDTABLE_H
#define __FDTABLE_H

struct Entry {
	unsigned int fileAddress;
	char mode; // r = read, w = write, a = append
};

class fdTable {

	private:
		Entry *openFiles;
		unsigned int tableCapacity;
		unsigned int tableCount;

		unsigned int tableInsert(Entry file);
	public:
		fdTable(unsigned int tableCapacity = 2);
		~fdTable();

		openf(unsigned int address, char mode);
		closef();
};

#endif
