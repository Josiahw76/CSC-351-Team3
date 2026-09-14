// CSC 351 Team 2 - Fall 2026
// Template written by Josiah W.


class node {
	private:
		int pid; // -1 is free node, >= 0 is node with PID of that number
		unsigned int start; // starting memory block id
		unsigned int length; // number of blocks represented by this node
		node *prev; // Left pointer, if first it's null
		node *next; // Right pointer, if last it's null
	
	public:
		node(int pid, unsigned int start, unsigned int length);

};
class memManager {

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
