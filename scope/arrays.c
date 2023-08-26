#include <stdio.h>
#include <stdlib.h>

static void print_apple (const char *apple) 
{
	printf ("apple's address in print_apple() is: %p, and value is: %s\n", (void *) apple, apple);
}

int main (void) 
{
	static const char apple[] = "Hello World";
	
	printf ("apple's address in main() is: %p, and value is: %s\n", (void *) apple, apple);
	print_apple (apple);
	return EXIT_SUCCESS;
}
