#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv, char **envp) 
{
	for (size_t i = 0; environ[i]; i++) {
		printf("%zu: %s\n", i, environ[i]);
	}

	return EXIT_SUCCESS;
}
