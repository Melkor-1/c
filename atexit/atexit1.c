#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
*  Is this clever or obscure? As atexit is incapable of taking any arguments,
*  we call another function inside it that does all the cleaning, and return.
*/
static void atexit_clean(void *data);

static void clean(void) 
{
	atexit_clean(NULL);
}

static void atexit_clean(void *data) 
{
	static void *x;

	if (data) {
		x = data;
		atexit(clean);
	} else {
		free(x);
	}
}

int main(void) 
{
	const size_t *const foo = malloc(sizeof(*foo));
	if (!foo) {
		errno = ENOMEM;
		perror("malloc");
		return EXIT_FAILURE;
	}
	atexit_clean(foo);
	return EXIT_SUCCESS;
}

