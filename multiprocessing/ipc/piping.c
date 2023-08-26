#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (void) 
{
	int pfds[2] = {0};
	errno = 0;

	if (pipe(pfds) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	
	errno = 0;
	int rv = fork();
	if (rv == -1) {
		perror("fork");
		return EXIT_FAILURE;
	}

	if (!rv) {
		/* Close normal stdout */
		close(STDOUT_FILENO);

		/* Make the write-end of the pipe same as stdout */
		if (dup(pfds[1]) == -1) {
			perror("dup");
			_exit(EXIT_FAILURE);
		}
		/* We don't need the read-end of the pipe */
		close(pfds[0]);
		
		errno = 0;
		if (execlp("ls", "ls", (char *) 0) == -1) {
			perror("execlp");
			_exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	} 

	/* Close normal stdin */
	close(STDIN_FILENO);
	errno = 0;

	/* Make the read-end of the pipe same as stdin */
	if (dup(pfds[0]) == -1) {
		perror("dup");
		return EXIT_FAILURE;
	}
	/* We don't need the write-end of the pipe */
	close(pfds[1]);
	errno = 0;

	if (execlp("wc", "wc", "-l", "-m", "-w", (char *) 0) == -1) {
		perror("execlp");
		return EXIT_FAILURE;
	}
	wait(0);
	return EXIT_SUCCESS;
}
			

