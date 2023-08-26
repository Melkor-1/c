#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define MAX_ALLOCS 10000000

int main(void) 
{
	fprintf(stderr, "\n============BEFORE MALLOC===============");
	malloc_stats();

	fprintf(stderr, "\n============AFTER MALLOC================");
	char *const address = malloc(MAX_ALLOCS);
	if (!address) {
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}
	memset(address, 0x0D, 1);
	malloc_stats();
	free(address);

	fprintf(stderr, "\n============BEFORE MMAP================");
	malloc_stats();


	fprintf(stderr, "\n=============AFTER MMAP================");
	char *const new = mmap(0, 3 * MAX_ALLOCS, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	memset(new, 0x0E, 1);
	malloc_stats();
	if (munmap(new, MAX_ALLOCS) == -1) {
		perror("munmap");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
