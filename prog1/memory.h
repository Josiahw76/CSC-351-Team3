// Template written by Josiah W.

#ifndef __MEMORY_H
#define __MEMORY_H

// forward declarations
class node;
class list;
class memManager;

// Use these for simple math
const unsigned int KB = 1024;
const unsigned int MAX_MEM = 256 * KB;
const int BLOCK_COUNT = 128;

// Policy variables
const unsigned int FIRST = 1;
const unsigned int NEXT = 2;
const unsigned int BEST = 3;
const unsigned int WORST = 4;


class memManager {
	private:
        node *memRoot; // Points to the first node in DLL structure
        node *memNext; // Points to the node where search for next fit begins
        // unsigned int start, length; // ??? ASK SWHEAT ??? 
        
		unsigned int nodeCount;

        // 1-4 for fit policies first, next, best, and worst respectively
        unsigned int policy; 
        
    public:
        // Constructor that builds an object of blockCount memory blocks which
        // uses the given policy
        memManager(unsigned int policy, unsigned int blockCount); 
        ~memManager(); // Destructor: deallocates all dynamic memory in use
        
        // Allocates num_units units of memory to a process whose id is 
        // process_id. If successful, it returns the number of nodes traversed 
        // in the linked list. Otherwise, it returns -1
        int allocMem(int process_id, int num_units);
        
        // If the process indexed by process_id has any memory allocated to it,
        // deallocates that memory
        void deallocMem(int process_id);
        
        // Checks integrity of DLL for improper linkages and length. If list is
        // out of order, the program shall exit with error message identifying
        // the node where the issue was found and the number of nodes that were
        // on the list. The first node should have prev pointer to NUll while
        // the last node should have a next pointer to NULL
        void checkLL();
        
        // Prints all the nodes' attributes in the DLL, one node per line
        void printIt();
};

class node {
	private:
		int pid; // -1 is free node, >= 0 is node with PID of that number
		unsigned int start; // starting memory block id
		unsigned int length; // number of blocks represented by this node
		node *prev; // Left pointer, if first it's null
		node *next; // Right pointer, if last it's null
		unsigned int start, length; // tracks memory blocks
		node *prev, *next; // DLL left/right pointers
	
	public:
		node(int pid, unsigned int start, unsigned int length,
						node *prev = NULL, node *next = NULL);

	friend memManager; // Friends are ones who can access your private members
};

// It's a list(!)
// what does he even do
class list {
	private:
		unsigned int *a; // actual contents of the list
		unsigned int listCount, listCapacity;
	public:
		list(unsigned int listCapacity);
		// Important behavior to note: a capacity of zero means no array.
		// serves you right. Asking for an empty list smh
		~list();
		bool add(int val);
		bool deleteAt(unsigned int index);
		int readAt(unsigned int index);
		void printIt() const;
		unsigned int getCount();
};

#endif
