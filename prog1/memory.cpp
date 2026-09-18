#include "memory.h"
#include <iostream>

//******************************************************************************
//				Node Function				      //
//******************************************************************************

node::node(int pid, unsigned int start, unsigned int length, node *prev, node *next) {
	this->pid = pid;
	this->start = start;
	this->length = length;
	this->prev = prev;
	this->next = next;
}

//******************************************************************************
//				List Functions				      //
//******************************************************************************

// Author: Josiah W

list::list(unsigned int listCapacity) {
	this->listCapacity = listCapacity;
	listCount = 0;

	// If capacity is zero, don't bother allocating anything
	if (listCapacity > 0) {
		a = new unsigned int[listCapacity];
	} else {
		a = NULL;
	}
}

//******************************************************************************

// Josiah W
list::~list() {
	if (a != NULL) { delete [] a; } // If a exists, delete it.
}

//******************************************************************************

// Josiah W
bool list::add(int val) {
	bool rc = listCount < listCapacity;
	if (rc) {
		a[listCount] = val;
		listCount++;
	}
	return rc;
}

//******************************************************************************

// Josiah W
bool list::deleteAt(unsigned int index)
{
	bool rc = (index < listCount);
	if (rc)
	{
		// decrement before to account for zero indexing
		listCount--;
		a[index] = a[listCount];
	}
	return rc;
}

//******************************************************************************

// Josiah W
int list::readAt(unsigned int index) const
{
	int rc = -1; // return -1 if read failed for any reason
	if (index < listCount)
	{
		rc = a[index];
	}
	return rc;
}

//******************************************************************************

// Josiah W
void list::printIt() const {
	for (unsigned int i = 0; i < listCount; i++) {
		std::cout << a[i] << std::endl;
	}
}

//******************************************************************************

// Josiah W
unsigned int list::getCount() {
	return listCount;
}

//******************************************************************************


//******************************************************************************
//				memManager Functions			      //
//******************************************************************************


//******************************************************************************

// Author: Dylan P - 9/17

memManager::memManager(unsigned int policy, unsigned int blockCount) {
    this->policy = policy; // Indicates which fit-policy will be used.

	this->blockCount = blockCount; //Indicates how many blocks of memory are available for allocation.
    
    // Enough nodes to fill storage with 3-block units.
    nodeCount = blockCount/3; 
    
    PIDlist = new list(nodeCount); // At most, one process per node.
    
    memRoot = new node(-1, 0, 0); // Root of DLL, linked to null on both sides.
    memNext = memRoot; // Initially, the next fit pointer starts at first block.
    
    node *p = memRoot; // Initialize prev pointer for loop.
    node *n; // Declares next pointer for.
    
    for (unsigned int i = 1; i < nodeCount; i++) {
    // Iteratively creates the remaining number of needed nodes.
        // Creates new node, backward linked to its predecessor.
        p = new node(-1, 0, 0, p); 
        
        // Forward links the node before new node to the new node
        p->prev->next = p; 
    }
    
    p->next = NULL; // Forward links last node in DLL to NULL
}

//******************************************************************************

// Author: Josiah W - 9/17

memManager::~memManager() {
	// can't really do a recursive deletion here, so
	// conditional loop it is.
	node *p = memRoot;

	while (p->next != NULL) {
		p = p->next;
		delete p->prev;
	}
	delete p;
}

//******************************************************************************

// Author: Dylan P - 9/17

int memManager::allocMem(int process_id, int num_units) {
    int traversalCount = 0; // Return number of nodes traversed, or -1 on fail
    
    node *p; // Points to the current working node
    
    // Node at which blocks might be allocated if subsequent space is sufficient
    node *candidate; 
    
<<<<<<< HEAD
    unsigned int left; // Index of first free block
    unsigned int right; // Index of last free block
=======
    int left; // Index of first free block
    int right; // Index of last free block
    int offset; // Number of free blocks
    int minOffset; // Used in best fit
    node *bestFit = NULL; // Used to save place of best fit block space
>>>>>>> 8e371137e83e0a80ce7890eeee077f2be934a452
    
    switch (policy) {
    // Allocates according to specified fit-policy
    
        case FIRST: // Author: Dylan P - 9/18
            p = memRoot; // Start search for space at beginning of DLL 
            
            do {
            // Repeats until the end of list, or until space is found
            
                while (p->next && p->next->pid > -1) {
                // Find next unallocated node
                    p = p->next;
                    
                    // Tracks number of nodes traversed before space is found
                    traversalCount++; 
                }
                
                // Set left bound to the first free block (the block directly
                // after the last allocated node)
                left = p->start + p->length; 
                
                if (p->next) {
                // If not at end of list...
                
                    // The candidate is the first unallocated block
                    candidate = p->next;
                    
                    while (p->next && p->next->pid < 0) {
                    // Find next allocated node
                        p = p->next;
                        
                        // We don't update traversal here because the candidate
                        // is staying put, and that's where we'll allocate space
                    }
                    
                    if (p->next) {
                    // If not at the end of list...
                    
                        // Set right bound to the start of allocated node blocks
                        right = p->next->start;
                    } else {
                    // Reached the end of list
                    
                        right = blockCount; // Set right bound to end of blocks
                    }
                    
                    if (right - left >= num_units) {
                    // Sufficient space is found to allocate!
                    
                        // Populate candidate's node fields with proper values
                        candidate->pid = process_id; 
                        candidate->start = left;
                        candidate->length = num_units;
                        break; // Exit the searhch, we found what we came for
                    }
                    
                } else {
                // Reached the end of DLL without finding unallocated node
                
                    traversalCount = -1; // There is no space to allocate, stop
                    break;               // looking for space and return failure
                }          
                
            } while (p->next); // Stop looping at end of list (p->next = NULL)
     
            break; // End of first fit policy logic
        
        case NEXT: // Author: Dylan P - 9/18
            p = memNext; // Begin search at the saved next-fit position
            
            do {
            // Repeat until space is found or whole DLL is searched
                while (p && p->pid > -1) {
                // Find next unallocated node
                    if (p->next) {
                        p = p->next; // Move to next node if not at end of DLL
                    } else {
                        p = memRoot; // Move to start if at end of DLL
                    }
                    
                    traversalCount++; // Account for traversed node
                    
                    if (p == memNext) {
                        traversalCount = -1; // Could not find spacce in DLL
                        break;
                    }
                }
                
                candidate = p; // We might be able to allocate at this node
                
                while (p && p->pid < 0) {
                // Find next allocated node
                    if (p->next) {
                        p = p->next; // Move to next node if not at end of DLL
                    } else {
                        p = memRoot; // Move to start if at end of DLL
                    }
                    if (p == memNext) {
                        break;
                    }
                }
                
                // Left bound is directly after the node before candidate
                left = candidate->prev->start + candidate->prev->length;
                
                // Right bound is the beginning of next allocated node
                right = p->start; 
                
                if (right - left >= num_units) {
                // Sufficient space is found!
                    // Populate candidate's node fields with proper values
                    candidate->pid = process_id; 
                    candidate->start = left;
                    candidate->length = num_units;
                    memNext = candidate; // Next search starts after this node
                    break; // Exit the search, we got what we came for!
                }
                
            } while (traversalCount > -1); 
            // Ends once every node up until memNext is checked
            
            break; // End of next-fit policy
            
        case BEST: // Author: Dylan P - 9/18        
            // Starter value to compare against, any offset will be smaller
<<<<<<< HEAD
            unsigned int minOffset = blockCount; 
=======
            minOffset = blockCount; 
>>>>>>> 8e371137e83e0a80ce7890eeee077f2be934a452
            
            p = memRoot; // Start search at beginning of DLL
            
            do {
            // Loops over entire DLL to find the best fit
            
                while (p->next && p->next->pid > -1) {
                // Find next unallocated node
                    p = p->next;
                    traversalCount++; // Account for the traversed node
                }
                
                // Set left bound to the end of blocks last used node allocated
                left = p->start + p->length; 
                
                if (p->next) {
                // If we aren't at end of list
                    
                    candidate = p->next; // We might be able to insert here
                                
                    while (p->next && p->next->pid < 0) {
                    // Find next allocated node
                        p = p->next;
                    }
                    
                    // Right bound is the start of next allocated node
                    right = p->start; 
                    
                    // The offset represents how many blocks are available
                    offset = right - left;
                    
                    if (offset = num_units) {
                    // The tightest space is found
                        // Populate candidate's node fields with proper values
                        candidate->pid = process_id; 
                        candidate->start = left;
                        candidate->length = num_units;
                        
                        // No need to continue searching for a more perfect fit
                        break; 
                    } 
                    
                    if (offset > num_units && offset < minOffset) {
                    // If the offset can fit the requested blocks, and is a 
                    // tighter fit than our last candidate, update bestFit
                        bestFit = candidate; // Our current best option
                        minOffset = offset; // Tighten standard
                    }
                } else {
                // We looked through the whole list and didn't find any space
                    traversalCount = -1; // Failure code :(
                    break;
                }
                
            } while (p->next); // No need to continue, nothing left to search
            
            if (bestFit) {
            // If we found a fit...
                // Populate the best fit's node fields with proper values
                bestFit->pid = process_id; 
                bestFit->start = left;
                bestFit->length = num_units;
            }
            
            break; // End of best fit logic
            
		case WORST:
			// Josiah
       		// Finds the largest hole that fits our memory unit and
			// greedily allocates it

			node *lp; // lookahead
			p = lp = memRoot; // start both at root

			int greatest_size = 0;
			int offset;
			left = p->start + p->length; // init left side

			// Move ahead until a gap is discovered
			// gap is start2 - (start1 + length1)
			//
			// have to move forward pointer until a real node is found
			// once we find a measurable gap, record what we find

			while (true) {
				do {
					lp = lp->next;
					traversalCount++;
				} while (lp != NULL && lp->pid < 0)
				// Now, lp is either not real or has found an allocated node
					if (lp->pid > 0) {
					offset = lp->start - left;
					if (offset > greatest_size) {
						greatest_size = offset;
						candidate = p;
					}
				} else {
					// Else means that lp was null, so we have to break
					offset = 128 - left; // Check how far we are from the 
										 // true end of the blocks
					// Run the same size check logic
					if (offset > greatest_size) {
						greatest_size = offset;
						candidate = p;
					}
					break;
				}
				// Set our new left side to the current node
				// and start the search again
				p = lp;
				left = p->start + p->length;
<<<<<<< HEAD
			}

			while (p) {
=======
			} while (p);
>>>>>>> 8e371137e83e0a80ce7890eeee077f2be934a452
				
            break;
    }
    return traversalCount;
}

//******************************************************************************

// Author: Josiah

unsigned int memManager::countHoles() {
	int count = 0;
	int start1 = 0;
	int offset;
	node *p = memRoot; // Start at the root of the DLL
	while (p != NULL) {
		while (p->pid < 0 && p->next != NULL) {
			p = p->next;
		}

		// Moved forward: either next is null or current is allocated
		if (p->pid >= 0) {
			offset = p->start - start1;
			if ((offset > 0) && (offset < 3)) {

				// Offset indicates unallocated blocks between neighboring nodes
				// between 0 and 3 means 1 or 2, which is a fragment
				count++;
			}
			start1 = p->start + p->length; // Calibrate start position
		}
		p = p->next;
	}
	// Simeon K
	if (blockCount > start1) // If the last node is not at the end of the memory blocks, check for the remaining blocks
		{
			offset = blockCount - start1; // offset between the last node and the end of the memory blocks
			if (offset == 1 || offset == 2) // it means there are 1 or 2 unallocated blocks at the end of the memory blocks, which is a fragment
			{
				count++; // Increment the count of fragments
			}
	}
	return count;
}

//******************************************************************************

// Author - Josiah

void memManager::deallocMem(int process_id) {
	// This should just find all blocks with pid = process_id and
	// assign that value to -1
	int index = process_id; // make it easier to read
	
	int pid = PIDlist->readAt(index); // translate index to process_id
	node *p = memRoot;

	while (p->pid != pid && p->next != NULL) {
		p = p->next;
	}
	// We've found the unwilling contestant!
	if (p->pid == pid) {
		p->pid = -1;
		PIDlist->deleteAt(index);
	}
}

//******************************************************************************

// Kam - 9/17: This outputs the first hole that is big enough,
//while searching the list for a new hole.
void memManager::checkLL() { 
	node *p = memRoot; // Start at the root 
	node *prev = NULL; // Initialize prev pointer to NULL
	unsigned int visited = 0; // Count of nodes visited
	while (p != NULL) { // While we haven't reached the end

		if (visited >= nodeCount) {
			std::cerr << "Error: DLL visited length exceeds expected node count of " << nodeCount << std::endl; // If we have visited more nodes than expected, print error message and exit
			exit(EXIT_FAILURE); // Exit the program with failure status
		}

		if (p->prev != prev) { // Check if the prev pointer of the current node is correct
			std::cerr << "Error: Improper linkage at node " << visited << ". Expected" << nodeCount << " nodes." << std::endl; // If not, print error message and exit
			exit(EXIT_FAILURE); //
		}

		prev = p; // Update prev pointer to current node
		p = p->next; // Move to the next node
		visited++; // Increment count of nodes visited
	}

	if (visited != nodeCount) { // Check if the number of nodes visited is equal to the expected node count
		std::cerr << "Error: DLL length mismatch. Expected " << nodeCount << ", but visited " << visited << std::endl; // If not, print error message and exit
		exit(EXIT_FAILURE); // Exit the program with failure status

	}
}

//******************************************************************************

// Author: Simeon K - 9/18

void memManager::printIt() // Prints all the nodes' attributes.
{
    node *p = memRoot; // Start at the root of the DLL
    while (p != NULL) 
    {
        std::cout << "PID: " << p->pid << ", Start: " << p->start << 
        ", Length: " << p->length << ", Prev: " << p->prev << ", Next: " << 
        p->next << std::endl; // Print the attributes of the current node
        p = p->next; // Move to the next node in the DLL
    }
}


