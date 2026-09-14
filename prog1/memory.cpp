#include "memory.h"


//******************************************************************************
//				Node Function				      //
//******************************************************************************

node::node(int pid, unsigned int start, unsigned int length, node *prev = NULL, node *next = NULL) {
	this.pid = pid;
	this.start = start;
	this.length = length;
	this.prev = prev;
	this.next = next;
}

//******************************************************************************
//				List Functions				      //
//******************************************************************************

list::list(unsigned int listCapacity) {}


list::~list() {}


bool list::add(int val) {}


bool list::deleteAt(unsigned int index) {}


int list::readAt(unsigned int index) {}


void list::printIt() {}


unsigned int list::getCount() {}

//******************************************************************************
//				memManager Functions			      //
//******************************************************************************

