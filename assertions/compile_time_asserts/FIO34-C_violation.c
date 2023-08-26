#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

int main(void) 
{
	int c;

	do {
		c = getchar();
		static_assert(UCHAR_MAX < UINT_MAX, "FIO34-C violation");
	} while(c != EOF);

	return EXIT_FAILURE;
}
