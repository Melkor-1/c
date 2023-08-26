#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open");
		return EXIT_FAILURE;
	}

	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		perror("stat");
		return EXIT_FAILURE;
	}
	
	uintmax_t size = sb.st_size;
	printf("%ju\n", size);
	return EXIT_SUCCESS;
}

	
