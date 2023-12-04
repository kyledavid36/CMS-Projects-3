
//comparing bits & pointer to pointer is a 2d array.


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "VoteOn.h"


int VoteOn(void* Instances[], int nInstances, int nSize) {

	int i,j;
	int MaxRepeats = 0; 
	int maxIndex = -1;		// nothing is in the majority.
	int curRepeats;

	for (i = 0; i < nInstances; i++) {
		curRepeats = 0;
		for (j = i + 1; j < nInstances;j++) {

			if (memcmp(Instances[i], Instances[j], nSize) == 0) {

				curRepeats++;

			}
			if (curRepeats>MaxRepeats) {
			
				MaxRepeats = curRepeats;
				maxIndex = i;				//current index
			
			}
		}
	}

	return (maxIndex);
}