/*	Both the threads access the shared counter at the same time, which results in a race
*   condition. It takes three instructions to increment the counter, one to load the value 
*   of the counter from memory into a register, one to increment it, and one to store it
*	back into memory. Because these three instructions do not execute atomically (all at 
*   once), strange things can happen, and the results are unexpected and inconsistent. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0;
long loops;

void *worker(void *arg) 
{
	for (size_t i = 0; i < loops; i++) {
		counter++;
	}
}

int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		fprintf(stderr, "Usage: threads <value>\n");
		return EXIT_FAILURE;
	}
	loops = strtol (argv[1], NULL, 10); 
	pthread_t p1, p2;
	printf("Initial value: %d\n", counter);

	pthread_create(&p1, 0, worker, 0);
	pthread_create(&p2, 0, worker, 0);
	pthread_join(p1, 0);
	pthread_join(p2, 0);
	printf("Final value: %d\n", counter);
	return EXIT_SUCCESS;
}
	
