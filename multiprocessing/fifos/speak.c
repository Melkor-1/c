#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define FIFO_NAME "maid"

#define PROG_NAME "speak"

int main (void) 
{
	char s[BUFSIZ] = {0};

	errno = 0;
	if (mknod (FIFO_NAME, S_IFIFO | 0666, 0)) {
		perror ("mknod");
		return EXIT_FAILURE;
	}

	errno = 0;
	int fd = open (FIFO_NAME, O_WRONLY);
	if (fd == -1) {
		unlink (FIFO_NAME);
		perror ("open");
		return EXIT_FAILURE;
	}

	printf ("Waiting for readers...\n");

	errno = 0;
	int rv = 0;
	while (fgets (s, sizeof s, stdin)) {
		REPEAT:
		rv = write (fd, s, strlen (s));
		if (rv == -1) {
			if (errno == EINTR) {
				goto REPEAT;
			}
			perror ("write");
		}
		printf ("%s: wrote %d bytes.\n", PROG_NAME, rv);
	}
	return EXIT_SUCCESS;
}

