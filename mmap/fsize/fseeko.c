#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
	
	if (argc != 2) {	
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	if (fseeko(fp, 0, SEEK_END) == -1) {		/* Seek to the end of the file */
		perror("fseek");
		return EXIT_FAILURE;
	}
	
	off_t size = ftello(fp);					/* get current file pointer */
	if (size == -1) {	
		perror("ftello");
		return EXIT_FAILURE;
	}

	printf("%lu\n", size);
	return EXIT_SUCCESS;
}


