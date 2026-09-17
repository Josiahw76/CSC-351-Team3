#include "memory.h"
#include <stdlib.h>

using namespace std;

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

// Random number generator
double rng() {
	return rand() % 100; // output value is a whole number between 0 and 100
}

bool do_alloc(int percentageOfAllocs) {
	
	return
}

/******************************************************************************/

memManager *man1, *man2, *man3, *man4;

int main(int argc, char *argv[]) {
    int rc = -1;

    // argv has 4 elements
    // ./memSim			0
    // numberOfRequests		1
    // percentageOfAllocs	2
    // randomSeed		3

	int numberOfRequests = reconstruct_int(argv[1]);
	int percentageOfAllocs = reconstruct_int(argv[2]);
	int randomSeed = reconstruct_int(argv[3]);

	srand(randomSeed); // Initialize the random number generator

	// Instantiate one object per policy.
	// These variables are defined in memory.h
	man1 = new memManager(FIRST, BLOCK_COUNT);
	man2 = new memManager(NEXT, BLOCK_COUNT);
	man3 = new memManager(BEST, BLOCK_COUNT);
	man4 = new memManager(WORST, BLOCK_COUNT);
	
	// Our test subjects
	memManager **Sims = {man1, man2, man3, man4};

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < numberOfRequests; j++) {
			Sims[i];		
	
		}
	}

    return rc;
}

