#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
	printf("Hello world from process ID %jd\n", (intmax_t) getpid());
	return EXIT_SUCCESS;
}
