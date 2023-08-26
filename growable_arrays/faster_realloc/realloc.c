#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 128

int main(void) 
{
	double *values = NULL;
	size_t nvals = 0;
	size_t buf_len = 0;
	double value = 0;
	double *new = 0;

	printf("Just keep trying in numbers.\n");
	while (scanf("%lg", &value) == 1) {
		if (nvals == buf_len) {
			buf_len += BUFSIZE;
			new = realloc(values, buf_len * sizeof *values);
		}
		if (!new) {
			free(values);
			perror("realloc");
			return EXIT_FAILURE;
		}
		values = new;
		new = 0;			/* Eliminates a dangling pointer */
		values[nvals] = value;
		nvals++;
	}
	printf("This array has %zu elements and %zu spare\n", nvals, buf_len - nvals);
	
	for (size_t i = 0; i < nvals; i++) {
		printf("%zu : %lg\n", i, values[i]);
	}
	free(values);
	return EXIT_FAILURE;
}

/* A more subtle problem :
*  This approach if fine if we only wanted to use the array inside a single function 
*  but hhow will we pass its information between functions ?
*/
