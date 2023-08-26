#define NDEBUG		/* This macro disenables the assertions. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

/* Expects a null-terminated string with its lenght, as determined by strlen.
*  Returns a char * on success. 
*  Returns 0 if size or src is NULL. 
*  Returns 0 on malloc failure.
*  Caller is responsible for freeing the returned char *.
*/
static char *dup_str(size_t size, const char *src) 
{
	assert (size > 0 && "size is zero or negative.");
	assert (s && "s is null");

	if (size <= 0 || !src) {
		return 0;
	}
	char *dst = malloc(size + 1);
	return dst ? memcpy(dst, src, size + 1) : NULL;
}

int main(void) 
{
	const char *const s = "Duplicate me!";
	const char *t = NULL;
	size_t size = strlen(s);	
	t = dup_str(size, s);
	
	if (!t) {
		perror("malloc ");
		return EXIT_FAILURE;
	}

	printf("The duplicated string is [%s].\n", t);
	free( (void *) t);

	return EXIT_SUCCESS;
}
