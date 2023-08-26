#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
		return EXIT_FAILURE;
	}
	DIR *dp;
	struct dirent *dirp;

	if (!(dp = opendir(argv[1]))) {
		perror(argv[0]);
		return EXIT_FAILURE;
	}
	errno = 0;
	while (dirp = readdir(dp)) {
		printf("%s\n", dirp->d_name);
	}
	
	if (errno) {
		perror(argv[0]);
		goto CLEANUP;
	}

	CLEANUP:
	if (closedir(dp) == -1) {
		perror(argv[0]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
	
