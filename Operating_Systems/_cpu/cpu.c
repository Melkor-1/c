#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		fprintf(stderr, "Usage: cpu <string>\n");
		return EXIT_FAILURE;
	}

	const char *const str = argv[1];
	while (true) {
		sleep(1);
		printf("%s\n", str);
	}
	return EXIT_SUCCESS;
}
