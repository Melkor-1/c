#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
	if (argc != 2 ) {
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
		perror("fstat");
		return EXIT_FAILURE;
	}

	char *address = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (address == MAP_FAILED) {
		perror("mmap");
		return EXIT_FAILURE;
	}
	
	for (size_t i = 0; i < sb.st_size; i++) {
		printf("%c", address[i]);
	}
	
	if (munmap(address, sb.st_size) == -1) {
		perror("munmap");
		return EXIT_FAILURE;
	}

	if (close(fd) == -1) {
		perror("close");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
