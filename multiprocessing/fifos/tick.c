#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#define FIFO_NAME "maid"

#define PROGRAM_NAME "speak"

int main (void) 
{
	char s[BUFSIZ] = {0};

	errno = 0;
	if (mknod (FIFO_NAME, S_IFIFO, 0) == -1) {
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

	printf ("Got a writers...\n");

	errno = 0;
	int rv = 0;
	
	do {
		if ((rv = read (fd, s, sizeof s)) == -1) {
			if (errno == EINTR) {
				continue;
			}
			perror ("read");
		} else {
			s[rv] = '\0';
			printf ("%s: read %d bytes: \"%s\"\n", PROGRAM_NAME, rv, s);
		}
	} while (rv > 0);
	
	return EXIT_SUCCESS;
}

