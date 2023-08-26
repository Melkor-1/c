#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
	struct stat sb;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		return EXIT_FAILURE;
	}
	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		return EXIT_FAILURE;
	}
	size_t size = sb.st_size;
	printf("The size of the file is %zu\n", size);
	
	return EXIT_SUCCESS;
}
