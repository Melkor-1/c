#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) 
{
 	const char *const name = argv[0] ? argv[0] : "";
	char *prog_name = malloc(strlen(name) + 1);
	if (prog_name) {
		strcpy(prog_name, name);
	} else {
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <path>\n", prog_name);
		return EXIT_FAILURE;
	}
	free(prog_name);

	uintmax_t path_max;
#ifdef PATH_MAX 
	path_max = PATH_MAX;
#else
	path_max = pathconf(path, _PC_PATH_MAX);
	if (path_max <= 0) {
		path_max = 4096;
	}
#endif
	char *abs_path = realpath(argv[1], 0);
	if (!abs_path) {	
		perror("realpath");
		return EXIT_FAILURE;
	}
	puts(abs_path);
	int fd = open(abs_path, O_RDONLY);
	if (fd == -1) {
		perror("open");
		free(abs_path);
		return EXIT_FAILURE;
	}
	printf("File [%s] opened successfully.\n", abs_path);
	if (close(fd) == 1) {
		perror("close");
		free(abs_path);
		return EXIT_FAILURE;
	}

	free(abs_path);
	return EXIT_SUCCESS;
}

