#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
	printf("uid = %u, gid %u\n", getuid(), getgid());
	return EXIT_SUCCESS;
}
