#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int buffer (FILE *fp, char *buf, int type, size_t size) 
{
	errno = 0;
	if (setvbuf (fp, buf, type, size)) {
		fclose (fp);
		perror ("setvbuf");
		return EXIT_FAILURE;
	}
}

int main (void) 
{
	errno = 0;	
	FILE *const fp = fopen ("foo.txt", "w");
	if (!fp) {
		perror ("fopen");
		return EXIT_FAILURE;
	}
	errno = 0;
	char *const buf = malloc (1000);
	if (!buf) {
		errno = ENOMEM;
		perror ("malloc");
		return EXIT_FAILURE;
	}
	/* First time, full buffering */
	buffer (fp, buf, _IOFBF, sizeof buf); /* Took 0m0.038s */
	
	/* Second time, line buffering */
	buffer (fp, buf, _IOLBF, sizeof buf); /* Took 0m0.028s */

	/* Third time, no buffering */
	buffer (fp, 0, _IONBF, 0); 			  /* Took 0m0.062s */

	for (size_t i = 0; i < 1000; i++) {
		fputc ('f', fp);
	}
	free (buf);
	fclose (fp);
	return EXIT_SUCCESS;
}
