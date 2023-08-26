#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
	fprintf(stdout, "Dumping input to stdout.\n");
	char buf[BUFSIZ];

	while (fgets(buf, BUFSIZ, stdin)) {
		fprintf(stdout, "Input: %s\n", buf);
	}
	fprintf(stderr, "BTW, this is stderr.\n");
	return EXIT_SUCCESS;
}
	
