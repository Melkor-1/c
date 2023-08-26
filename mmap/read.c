#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

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
		perror("fstat");
		close(fd);
		return EXIT_FAILURE;
	}

	char *buffer = malloc(sb.st_size + 1);
	if (!buffer) {
		perror("malloc");
		close(fd);
		return EXIT_FAILURE;
	}

	if (read(fd, buffer, sb.st_size) == -1) {
		perror("read");
		close(fd);
		return EXIT_FAILURE;
	}

	if (write(1 , buffer, sb.st_size) == -1) {
		perror("write");
		close(fd);
		return EXIT_FAILURE;
	}
	
	free(buffer);
	if (close(fd) == -1) {
		perror("close");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
