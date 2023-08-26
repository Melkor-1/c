#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) 
{
	int pfds[2];
	errno = 0;

	if (pipe(pfds) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	char buf[BUFSIZ] = {0};
	int rv = 0;
	errno = 0;

	rv = fork();
	if (rv == -1) {
		perror("fork");
		return EXIT_FAILURE;
	}
	
	if (!rv) {
		/* Child process */
		/* We don't need the read-end side of the pipe */
		close(pfds[0]);		
		errno = 0;
	
		printf("CHILD: Writing to pipe.\n");
		if (write(pfds[1], "testing", 7) == -1) {
			perror("write");
			_exit(EXIT_FAILURE);
		}
		printf("CHILD: Exiting.\n");
		_exit(EXIT_FAILURE);
	}

	/* Parent process */
	/* We don't need the write-end side of the pipe */
	close(pfds[1]);
	errno = 0;
	
	printf("PARENT: Reading from pipe.\n");
	if (read(pfds[0], buf, 7) == -1) {
		perror("read");
		return EXIT_FAILURE;
	}
	printf("PARENT: read \"%s\"\n", buf);
	close(pfds[0]);
	wait(0);
	return EXIT_SUCCESS;
}
	
