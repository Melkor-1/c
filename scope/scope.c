#include <stdio.h>
#include <stdlib.h>

static void print_apple (int apple) 
{
	/* C passes variables by-value/by-copy, so the value in apple was copied 
	*  to the function's parameter.
	*/
	apple = 10;
	printf ("apple in print_apple() has address: %p and value %d\n", (void *) &apple, apple);
}

static void print_apple1 (int *apple) 
{
	/* But if we pass the address of the local variable instead, we can 
	*  override the default semantics. We simulated pass-by-reference, and 
	*  apple now contains the address of the variable in main(). Any change
	*  made to the apple's pointee will result in a change in main()'s apple.
	*/
	printf ("apple in print_apple1() has address: %p and value %p\n", (void *) &apple, (void *) apple);
	*apple = 100;
}

int main (void) 
{
	int apple = 5;
	printf ("apple in main() has address: %p and value %d\n", (void *) &apple, apple);
	print_apple (apple);
	printf ("apple in main() after print_apple() has address: %p and value %d\n", (void *) &apple, apple);
	print_apple1 (&apple);
	printf ("apple in main() after print_apple1() has address: %p and value %d\n", (void *) &apple, apple);
	return EXIT_SUCCESS;
}

