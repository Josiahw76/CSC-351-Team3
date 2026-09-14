// CSC 351 Team 2 - Fall 2026
// Template written by Josiah W.


// forward declarations so we can define memManager first
class node;
class list;

class memManager {
	private:
		node *memRoot; // first node of dll
		node *memNext;
		unsigned int start, length;
		unsigned int policy;
	public:
		memManager(unsigned int policy, unsigned int blockCount);
		// number of blocks in final submission is 128
		~memManager();
		int allocMem(int process_id, int num_units);
		void deallocMem(int process_id);
		unsigned int countHoles();
		void checkLL(); // check integrity of dll, panic if broken
		void printIt();

};

class node {
	private:
		int pid; // -1 is free node, >= 0 is node with PID of that number
		unsigned int start, length; // tracks memory blocks
		node *prev, *next; // DLL left/right pointers
	
	public:
		node(int pid, unsigned int start, unsigned int length,
						node *prev = NULL, node *next = NULL);

	friend memManager; // Friends are ones who can access your private members
};

class list {
	private:
		unsigned int *a; // actual contents of the list
		unsigned int listCount;
		unsigned int listCapacity; 
	public:
		list(unsigned int listCapacity); // constructor
		~list(); // destructor
		bool add(int val); // append the list with val: this is constant time
		bool deleteAt(unsigned int index); // like P1 list delAt, rc is success
		int readAt(unsigned int index); // read the val at index, otherwise -1
		void printIt() const; // print list, one entry per line; 4 debug
		unsigned int getCount(); // return listCount

};
