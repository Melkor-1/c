#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
	char *value = NULL;

	value = getenv("HOME");
	printf("HOME=%s\n", value);
	value = getenv("USER");
	printf("USER=%s\n", value);
	value = getenv("PASSWORD");
	printf("PASSWORD=%s\n", value);

	return EXIT_SUCCESS;
}
