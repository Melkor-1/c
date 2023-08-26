#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 108

/* Growable array of floats */
struct double_array {
	double *values;
	size_t nvals;
	size_t buflen;
};

/* Returns a pointer to a struct of type double_array on success.
*  Otherwise, it sets errno to ENOMEM and returns a NULL pointer.
*  Caller is responsible for deallocating the returned pointer.
*/
static struct double_array *new_double_array(void) 
{
	struct double_array *array;
	array = malloc(sizeof *array);	
	if (!array) {
		return 0;
	}
	array->nvals = array->buflen = 0;
	array->values = 0;
	return array;
}	

/* Adds a value to an array, extending it if necessary
*  Returns 1 on success, and 0 on failure. And sets errno
*  to ENOMEM.
*/
static size_t add_to_array(struct double_array *array, double value) 
{
	double *new = 0;
	/* Check to see if we need to extend the array */
	if (array->nvals >= array->buflen) {
		array->buflen += BUFSIZE;
		new = realloc(array->values, array->buflen * sizeof array->values);
	}
	if (!new) {
		return 0;
	}
	array->values = new;
	new = NULL;			/* Eliminates a dangling pointer */
	array->values[array->nvals] = value;
	array->nvals++;

	return 1;
}

int main(void) 
{
	struct double_array *array;
	double value;

	array = new_double_array();
	if (!array) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	printf("Just keep typing in numbers:\n");
	while (scanf("%lg", &value) == 1) {
		if (!add_to_array(array, value)) {
			free(array);
			perror("realloc");
			return EXIT_FAILURE;
		}
	}

	printf("This array has %zu elements\n", array->nvals);
	for (size_t i = 0; i < array->nvals; i++) {
		printf("%zu: %lg\n", i, array->values[i]);
	}

	free(array);
	return EXIT_SUCCESS;
}
