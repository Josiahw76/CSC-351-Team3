// Author: Josiah W

#include "memory.h"
#include <stdlib.h>

using namespace std;

int process_id = 1;
int allocation_attempts = 0;
int successful_allocs = 0;
int agg_hole_counter = 0;	
int agg_traversal_counter = 0;
int agg_failure_counter = 0;

int numberOfRequests;
int percentageOfAllocs;

memManager *man1, *man2, *man3, *man4;


// Helper functions
/******************************************************************************/

// Take in params and prep them for use
int reconstruct_int(char *str) {
    char a;
	int count = 0;
	int reconstructed_int = 0;

	while ((a = str[count]) != '\0') {
   	 	reconstructed_int *= 10;
    	reconstructed_int += a - '0';
    	count++;
		// Shift current val one left and append the number.
		// for 15: 00 <- 01 <- 10 <- 15
	}
	return reconstructed_int;
}

// Sally shall decide which operation to perform
bool consult_sally() {
	return (rand() % 100) < percentageOfAllocs;
}

// Get a value between 3 and 10 for memory allocation
int getBlockRNG() {
	return (rand() % 8) + 3;
}

// Statistical Functions:

int avg_hole_count() {
	return agg_hole_counter / successful_allocs;
}

int avg_traversals() {
	return agg_traversal_counter / successful_allocs;
}

double denial_percentage() {
	return agg_failure_counter / allocation_attempts;
}


/******************************************************************************/

int main(int argc, char *argv[]) {
    int rc = -1;

	numberOfRequests = reconstruct_int(argv[1]);
	percentageOfAllocs = reconstruct_int(argv[2]);

	srand(reconstruct_int(argv[3])); // Initialize the random number generator

	// Instantiate one object per policy.
	// These variables are defined in memory.h
	man1 = new memManager(FIRST, BLOCK_COUNT);
	// man2 = new memManager(NEXT, BLOCK_COUNT);
	// man3 = new memManager(BEST, BLOCK_COUNT);
	// man4 = new memManager(WORST, BLOCK_COUNT);
	
	// This is to help iterate with loops.
	
	// memManager **Sims = {man1, man2, man3, man4};
	memManager **Sims = new memManager*[1];
	Sims[0] = man1;

	//memManager *Sims[] = {man1, man2, man3, man4};


	int mem_rc;
	int raffle_winner;
	int unluckyPID;
	int listCount;
	
	//for (unsigned int i = 0; i < 4; i++) {
	unsigned int i = 0;
		for (int j = 0; j < numberOfRequests; j++) {

			// We must defer to Sally's judgement on such matters
			if (consult_sally()) {

				// getBlockRNG returns a value between 3 and 10
				mem_rc = Sims[i]->allocMem(process_id, getBlockRNG());

				// Check whether allocMem returned a failure code or real num
				if (mem_rc != -1) {
					agg_traversal_counter += mem_rc;

				} else {
					agg_failure_counter++;
				}

				process_id++;
				allocation_attempts++;

			} else {
				// Select the lucky winner (process getting terminated)		
				raffle_winner = 
						rand() % (Sims[i]->PIDlist->getCount() - 1);

				// Get the process id of our winner
				// unluckyPID = Sims[i]->PIDlist->a[raffle_winner];

				// We want the index instead of the process_id
				unluckyPID = raffle_winner;
				// Show him what he's won
				Sims[i]->deallocMem(unluckyPID);	

			}
			Sims[i]->countHoles();
		}
	// }

    return rc;
}

