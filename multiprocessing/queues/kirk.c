#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
	long mtype;
	char mtext[BUFSIZ];
};

int main (void) 
{
	struct my_msgbuf pmb = { 0 };
	key_t const key = ftok ("apple.txt", 'b');
	
	if (key == -1) {
		perror ("ftok()");
		goto fail;
	}

	int const msqid = msgget (key, 0666 | IPC_CREAT);
	
	if (msqid == -1) {
		perror ("msgget()");
		goto fail;
	}
	puts ("Enter lines of text, EOF to quit.");
	pmb.mtype = 1;

	while (fgets (pmb.mtext, sizeof pmb.mtext, stdin)) {
		size_t len = strlen (pmb.mtext);

		pmb.mtext[strcspn (pmb.mtext, "\n")] = '\0';

		if (msgsnd (msqid, &pmb, len, 0) == -1) {
			perror ("msgsnd()");
		}
	}
	
	if (msgctl (msqid, IPC_RMID, 0) == -1) {
		perror ("msgctl()");
		goto fail;
	}

	return EXIT_SUCCESS;

	fail:
		return EXIT_FAILURE;
}

