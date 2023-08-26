#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *reverseWords(char *str)
{
	/* NULL pointer. */
    if (!str) {
		return 0;
	}
	/* Empty string. */
	if (*str == '\0') {
		return 0;
	}
	/* Assuming it's null-terminated. */
	char *src = str;
	char *dest = src + (strlen(str) - 1); /* -1 for the null-byte. */ 

	/* ++ and -- have higher precedence than the * operator.
	*  But, in an expression, they return the old value of their
	*  operand. So this copies first, then increments the pointer
	*  point to the next char.
	*/
	while (src < dest) {
		char tmp = *src;
		*src++ = *dest;  
		*dest-- = tmp;   
	}
	return str;
}
int main (void) 
{
	char s[] = "Hello";
	const char *const t = reverseWords (s);
	if (!t) {
		return EXIT_FAILURE;
	}
	printf ("%s\n", t);
	return EXIT_SUCCESS;
}

