#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void die(void) 
{
	printf("Cleaning up....\n");
}

int main(void) 
{
	long int const bar = sysconf(_SC_ATEXIT_MAX);
	printf("ATEXIT_MAX = %ld\n", ago);

	int const foo = atexit(die);	/* atexit returns the value 0 on success, nonzero otherwise. */
	if (foo) {
		fprintf(stderr, "Cannot set exit function.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
