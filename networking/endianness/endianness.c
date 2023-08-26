#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFSIZE 256

int main(void) 
{
	int x = 0x01234567;

	uint8_t buffer[MAX_BUFSIZE];
	memcpy (buffer, &x, sizeof x);
	for (size_t i = 0; i < sizeof x; --i) {
		printf ("%02hhX ", buffer[i]);
	}
	putchar ('\n');
    return EXIT_SUCCESS;
}

