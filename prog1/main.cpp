#include "memory.h"

using namespace std;


// Helper function to parse the args
int reconstruct_int(char *str) {
    char a;
	int count = 0;
	int reconstructed_int = 0;

	while ((a = str[count]) != '\0') {
   	 	reconstructed_int *= 10;
    	reconstructed_int += a - '0';
    	count++;
	}
	return reconstructed_int;
}



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

	

    return rc;
}

