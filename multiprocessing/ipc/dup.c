#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main (void) 
{
	errno = 0;
	int fd = open ("log.txt", O_WRONLY | O_CREAT);
	if (!fd) {
		perror ("open");
		return EXIT_FAILURE;
	}
	/* We want stdout to point to this file, for logging. */
	errno = 0;
	int rv = 0;

	do {
		rv = dup2 (fd, STDOUT_FILENO);
	} while ((rv == -1) && (errno == EINTR));

	if (rv == -1) {
		close (fd);
		perror ("dup2");
		return EXIT_FAILURE;
	}
	/* We no longer need the original file descriptor */
	close (fd);

	/* Now writing to STDOUT_FILENO == LOG.TXT_FILENO */
	printf ("You're my sunshine now.\n");

	return EXIT_FAILURE;
}
	
		
