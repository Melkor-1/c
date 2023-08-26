#include <stdio.h>
#include <stdlib.h>

static void print_shakespeare (const char *shakespeare) 
{
	printf ("In print_shakespeare(), shakespeare has address: %p, and value \"%s\"\n", (void *) shakespeare, shakespeare);
}
int main (void) 
{
	const char *const shakespeare = "To be or not to be?";
	printf ("In main(), shakespeare has address: %p, and value \"%s\"\n", (void *) shakespeare, shakespeare);
	print_shakespeare (shakespeare);
	return EXIT_SUCCESS;
}
