#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <glob.h>

int main (int argc, char **argv) 
{
	if (!argv) {
		fprintf (stderr, "A NULL pointer was passed in.\n");
		return EXIT_FAILURE;
	}
	if (argc != 2) {
		fprintf (stderr, "Usage: %s wildcard.\n", argv[0] ? argv[0] : "");
		return EXIT_FAILURE;
	}
	glob_t glob_buf;
	int ret_val = 0;

	if (!(ret_val = glob (argv[1], 0, 0, &glob_buf))) {
		for (size_t i = 0; i < glob_buf.gl_pathc; i++) {
			fprintf (stdout, "%s\n", glob_buf.gl_pathv[i]);
		}
	} else if (ret_val == GLOB_NOSPACE) {
		perror ("malloc()");
		return EXIT_FAILURE;
	} else if (ret_val == GLOB_ABORTED) {
		perror ("read()");
		return EXIT_FAILURE;
	} else {
		fprintf (stderr, "Found no matches.\n");
	}
	return EXIT_SUCCESS;
}
		

