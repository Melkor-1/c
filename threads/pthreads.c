#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

static void *my_turn(void *arg) 
{
	int *iptr = malloc(sizeof(int *));
	*iptr = 5;

	for (size_t i = 0; i < 5; i++) {
		sleep(1);
		printf("My turn [%zu] %d\n", i, *iptr);
		(*iptr)++;
	}	
	return iptr;
}

static void your_turn(void) 
{
	for (size_t i = 0; i < 8; i++) {
		sleep(1);
		printf("Your turn [%zu]\n", i);
	}
}

int main(void) 
{	
	pthread_t new_thread;
	int *result;
	pthread_create(&new_thread, NULL, my_turn, NULL);
	
	your_turn();
	
	pthread_join(new_thread, (void *)&result);
	printf("Thread's done: result = %d\n", *(int *)result);
	return EXIT_SUCCESS;
}

