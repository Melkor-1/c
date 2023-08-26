#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/* Synopsis: Sets the program name, based on argv[0]. And checks argument count.
*  Returns: 0 on failure, 1 elsewise.
*/
static int usage(int argc, const char *argv[]) 
{
	/* The name the program is called with */
	const char *prog_name = 0;

	/* POSIX requires the invoking process to pass a non-NULL argv[0] */
	if (!argv[0]) {
		fprintf(stderr, "A NULL argv[0] was passed through an exec system call.\n");
		return 0;
	}

	/* Safe to initialise now */
	prog_name = argv[0];
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <string>\n", prog_name);
		return 0;
	}
	return 1;
}

static void check_err(int rv, const char *name) 
{
	if (rv == -1) {
		perror("name");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, const char *argv[]) 
{
	/* Sanity Check */
	if (!usage(argc, argv)) {
		return EXIT_FAILURE;
	}

	int pfds[2] = {0};
	errno = 0;
	check_err(pipe(pfds), "pipe"); 
	
	errno = 0;
	int rv = 0;
	check_err(rv = fork(), "fork");

	/* Child process */
	if (!rv) {
		/* Close normal stdout */
		close(STDOUT_FILENO);
		
		/* Make stdout same as pfds[1] */
		if (dup(pfds[1]) == -1) {
			perror("dup");
			_exit(EXIT_FAILURE);
		}

		/* We don't need the read-end of the pipe */
		close(pfds[0]);

		if (execlp("ls", "ls", (char *) 0) == -1) {
			perror("execlp");
			_exit(EXIT_FAILURE);
		}
		_exit(EXIT_SUCCESS);
	}
	
	/* Parent process */
	/* Close normal stdin */
	close(STDIN_FILENO);

	/* Make stdin same as pfds[0] */
	check_err (dup(pfds[0]), "dup");

	/* We don't need the write-end of the pipe */
	close (pfds[1]);

	check_err (execlp("grep", "grep", argv[1], (char *) 0), "execlp");
	wait(0);
	return EXIT_SUCCESS;
}


		

