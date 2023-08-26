#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main (void) 
{
	errno = 0;
	if (!setvbuf (stdout, 0, _IONBF, 0) 
	    || !setvbuf (stdin, 0, _IONBF, 0)) {
		perror ("setvbuf");
		return EXIT_FAILURE;
	}
	

	int c;
	while ((c = getc (stdin)) != EOF) {
		putc (c, stdout);
	}
	return EXIT_FAILURE;
}
