// CSC 351 Team 2 - Fall 2026
// Template written by Josiah W.

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
