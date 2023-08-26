#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) 
{
	/* Sanity check. POSIX requires the invoking process to pass a non-NULL
	*  argv[0].
	*/
	if (!argv[0]) {
		fprintf(stderr, "A NULL argv[0] was passed through an exec system call.\n");
		return EXIT_FAILURE;
	}
	/* Do not assume that argv[0] can not be NULL */
	const char *const name = argv[0] ? argv[0] : "";
	char *prog_name = malloc(strlen(name) + 1);	/* +1 for the '\0' byte */
	if (prog_name != NULL) {
		strcpy(prog_name, name);
	} else {
		/* Failed to allocate memory, recover */
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}
	printf("%s", prog_name);
	free(prog_name);
	return EXIT_SUCCESS;
}

