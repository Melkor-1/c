#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUF_SIZE 50

struct character {
	char buffer[MAX_BUF_SIZE];
	size_t health;
	bool isalive;
};

int main(void) 
{
	struct character char1, char2;
	char1 = (struct character) {.buffer = "Kiyan", .health = 100, .isalive = true};

	/* First method */
	strcpy(char2.buffer, char1.buffer);
	char2.health = char1.health;
	char2.isalive = char1.isalive;

	/* Second method */
	memcpy(&char2, &char1, sizeof(char1));

	/* Third method */
	char2 = char1;

	printf("char1: name %s --- health %zu --- isalive %d.\n", char1.buffer, char1.health, char1.isalive);
	printf("char2: name %s --- health %zu --- isalive %d.\n", char2.buffer, char2.health, char2.isalive);
	
	return EXIT_SUCCESS;
}
