#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int BUFSIZE = 4096;

int main(void) 
{
	FILE *pipe = popen("echo \"Hello\" | ./test", "r");
	if (!pipe) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	char buf[BUFSIZE];
	while (fgets(buf, BUFSIZE, pipe)) {
		printf("OUT: %s\n", buf);
	}
	
	if (pclose(pipe) == -1) {
		perror("pclose");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

