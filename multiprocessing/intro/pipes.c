#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* pipe() takes an array of teo ints as an argument. Assuming no errors, it connects
*  two file descriptors and returns them in the array. The first element of the array
*  is the reading-end of the pipe, the second is the writing one.
*/
int main(void) 
{
	int pfds[2];
	char buf[BUFSIZ];

	errno = 0;
	if (pipe(pfds) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	printf("Writing to the file descriptor: #%d\n", pfds[1]);
	if (write(pfds[1], "testing", 7) == -1) {
		perror("write");
		return EXIT_FAILURE;
	}

	printf("Reading from file descriptor: #%d\n", pfds[0]);
	if (read(pfds[0], buf, 8) == -1) {
		perror("read");
		return EXIT_FAILURE;
	}
	close(pfds[0]);
	close(pfds[1]);

	printf("read() read \"%s\"\n", buf);
	return EXIT_SUCCESS;
}


