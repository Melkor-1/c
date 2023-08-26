#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <signal.h>
#include <unistd.h>

void handler(int num) 
{
	write(STDOUT_FILENO, "I won't die!\n", 13);
}

int main(void) 
{
	struct sigaction act = { 0 };
	act.sa_handler = handler;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	while (true) {
		printf("Wasting your cycles %d\n", getpid());
		sleep(1);
	}
    EXIT_SUCCESS;
}

