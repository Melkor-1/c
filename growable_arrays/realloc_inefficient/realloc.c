#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void) 
{
	double *values = 0;
	size_t nvals = 0;
	double value;

	printf("Just keep trying in numbers.\n");

	while (scanf("%lg", &value) == 1) {
		double *new = realloc(values, (nvals + 1) * sizeof *values);
		if (!new) {
			free(values);
			errno = ENOMEM;
			perror("realloc ");
			return EXIT_FAILURE;
		}
		values = new;
		new = 0;		/* Eliminates a dangling pointer */
		values[nvals] = value;
		nvals++;
	}
	
	printf("The array has %zu elements\n", nvals);
	for (size_t i = 0; i < nvals; i++) {
		printf("%zu : %lg\n", i, values[i]);
	}
	free(values)
	return EXIT_FAILURE;
}

/* This is a very inefficient method as realloc() is slow anyway, and everytime 
*  we add an element to the array we have to copy it to the new one.
*/
