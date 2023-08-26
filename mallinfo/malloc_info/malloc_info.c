#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <malloc.h>
#include <sys/mman.h>

#define MAX_ALLOCS 1000000

int main(void) 
{	
	printf("===============BEFORE MALLOC================\n");
	if (malloc_info(0, stdout) == -1) {
		perror("malloc_info");
		return EXIT_FAILURE;
	}
	
	printf("===============AFTER MALLOC==================\n");
	char *const address = malloc(MAX_ALLOCS);
	if (!address) {
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}
	memset(address, 0x0A, 1);
	malloc_info(0, stdout);
	free(address);
	
	printf("================BEFORE MMAP==================\n");
	malloc_info(0, stdout);

	printf("================AFTER MMAP====================\n");
	char *const content = mmap(0, 3 * MAX_ALLOCS, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (content == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	memset(content, 0x0B, 1);
	malloc_info(0, stdout);
	if (munmap(content, MAX_ALLOCS) == -1) {
		perror("munmap");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;

}
