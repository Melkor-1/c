#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int main(int argc, char *argv[]) 
{
	int *const p = malloc(sizeof *p);
	assert (p);
	printf("(%d) memory address of p: %p\n", getpid(), (void *)p);

	*p = 0;
	while (true) {
		sleep(1);
		*p = *p + 1;
		printf("(%d) p: %d\n", getpid(), *p);
	}
	return EXIT_SUCCESS;
}
