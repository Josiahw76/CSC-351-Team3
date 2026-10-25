#ifndef __PIDPQ_H
#define __PIDPQ_H

// For PCB definition
#include "scheduler.h"

// Program 13 PQ transformed to accept PCB pointers.

class PQ {
    private:
	PCB **keys;
	unsigned int pqSize;
	unsigned int qCount;

	unsigned int parent(unsigned int index) const;

	unsigned int left(unsigned int index) const;

	unsigned int right(unsigned int index) const;

	void printIt(unsigned int ind, unsigned int count) const;

	void swap(PCB * &x, PCB * &y);

	void bubbleUp(unsigned int index);
	
	void heapify(unsigned int i);

    public:
	PQ(unsigned int n = 100); 

	~PQ(); 

	void printIt() const;

	bool enq(PCB *kv);

	bool deq(PCB * &kv);

	void clear();

	unsigned int count() const;
};

#endif
