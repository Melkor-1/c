#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/resource.h>

/* Returns: the maximum resident set size used in kilobytes on success, -1 otherwise */
static long get_mem_usage(void) 
{
	struct rusage myusage;

	if(getrusage(RUSAGE_SELF, &myusage) == -1) {
		return -1;
	}
	return myusage.ru_maxrss;
}

int main(void)
{
	long size = 0;
	long int const base_line = get_mem_usage();

	for (size_t i = 0; i < 100; i++) {
		char *p = malloc(1024 * 100);
		memset(p, 1, 1024 * 100);

		if ((size = get_mem_usage()) == -1) {
			perror("getrusage");
			return EXIT_FAILURE;
		}
		printf("Usage: %ld + %ld\n", base_line, size - base_line);
	}
	return EXIT_SUCCESS;
}
