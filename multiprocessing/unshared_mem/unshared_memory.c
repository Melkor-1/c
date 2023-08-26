#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <stdint.h>

int v = 5;

int main(int argc, char **argv) 
{
	uint8_t *const shared_memory = mmap(0, 4096, PROT_READ | PROT_WRITE,
								  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*shared_memory  = 34;

	if (!fork()) {
		*shared_memory = 20;
		v = 80;
	} else {
		wait(NULL);
	}
	printf("Not shared. %i\n", v);
	printf("Shared. %i\n", *shared_memory);
}
