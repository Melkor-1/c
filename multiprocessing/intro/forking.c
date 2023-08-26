#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) 
{
	if (fork() == 0) {
		printf("I am the child.\n");
		execl("/bin/ls", "ls", "-al", (char *)NULL);
		printf("After execlp\n");
	} else {
		printf("I am old and parental. | %d\n", getpid());
	}
	return EXIT_SUCCESS;
}
