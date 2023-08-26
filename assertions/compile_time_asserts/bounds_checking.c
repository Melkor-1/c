#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(void) 
{
	static const char prefix[] = "Error no:";

#define ARRAY_SIZE 14
	char str[ARRAY_SIZE];

	/* Ensure that str has sufficient spacce to store at least one additional
	*  character for an error code
	*/
	static_assert(sizeof(str) > sizeof(prefix),
				 "str must be larger than prefix");
	strcpy(str, prefix);
	printf("str: [%s] is located at [%p].\n", str, (void *) str);

	return EXIT_SUCCESS;
}


