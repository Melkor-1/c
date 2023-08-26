#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <termios.h>

int main(void) 
{
	int c;
	struct termios old, new;

	if (tcgetattr(STDIN_FILENO, &old) == -1) {
		perror("tcgetattr");
		return EXIT_FAILURE;
	}
	new = old;

	new.c_lflag &= (~ICANON & ~ECHO);		/* clears canonical and echo mode. */
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new) == -1) {
		perror("tcsetattr");
		return EXIT_FAILURE;				/* No changes were made, we can afford to exit here */
	}

	while ((c = getchar()) != EOF) {
		if (isalpha((unsigned char) c)) {
			putchar('&');
			continue;
		}
		if (isdigit((unsigned char) c)) {
			putchar('$');
			continue;
		}
		putchar('.');
	}

	if (tcsetattr(STDIN_FILENO, TCSANOW, &old) == -1) {	
		perror("tcsetattr");	/* What do we do now that we can't revert back to old settings? */
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
