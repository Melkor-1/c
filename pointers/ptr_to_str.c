#include <stdio.h>
#include <stdlib.h>

int main (void) 
{
	static const char s[] = "abcdefghi";	/* s is an array[10] of static const char */
	static const char t[] = "jklmnopqi"; /* t is an array[10] of static const char */
	static const char (*foo)[10] = &s;	/* foo is a pointer to an array[10] of static const char */
	printf ("%s\n", *foo);

	/* Now we can change foo to point to t */
	foo = &t;	
	printf ("%s\n", *foo); 

	static const char (*ret[10])[10] = { &s, &t };
	/* ret is an array[10] of pointers to array[10] of static const char */ 
	
	for (size_t i = 0 ; i < 2; i++) {
		printf ("%s\n", *ret[i]);
	}
	return EXIT_FAILURE;
}

