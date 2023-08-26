#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

void handler(int num) {
	write(STDOUT_FILENO, "I won't die\n", 13);
}

int main(void) 
{
	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	signal(SIGTSTP, handler);
		
	while (true) {
		printf("Wasting your cycles. %d\n", getpid());
		size_t size = 100;
		char *p = malloc(size);
		size += 2;
		sleep(1);
	}
}
