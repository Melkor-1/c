#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "add.h"

int main(void) 
{
	int const result = add(98, 102);
	if (!result) {
        #ifdef ERANGE
		errno = ERANGE;
		perror(" ");
        #endif
		return EXIT_FAILURE;
	}
	printf("%d\n", result);
	
	return EXIT_SUCCESS;
}
