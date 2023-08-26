#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char **argv) 
{
	if (!*argv) {
		fprintf (stderr, "The program was called with a NULL argv[0]");
		return EXIT_FAILURE;
	}

	struct flock fl = { 0 };
	errno = 0;

	fl.l_type   = F_WRLCK;	/* F_RDLCK, F_WRLCK, F_UNLCK    */
	fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
	fl.l_start  = 0;		/* Offset from l_whence 		*/
	fl.l_len 	= 0;		/* Lenght, 0 = to EOF 			*/
	fl.l_pid 	= getpid(); /* Out PID 						*/

	if (argc > 1) {
		fl.l_type = F_RDLCK;
	}
	int fd = open ("lockdemo.c", O_RDWR);
	if (fd == -1) {
		perror ("open()");
		return EXIT_FAILURE;
	}
	printf ("Press <RETURN> to try to get lock.\n");
	getchar ();
	printf ("Trying to get lock\n");

	errno = 0;
	if (fcntl (fd, F_SETLKW, &fl) == -1) {	/* Set the lock, blocking if necessary */
		perror ("fcntl");
		close (fd);
		return EXIT_FAILURE;
	}
	
	printf ("Got lock.\n");
	printf ("Press <RETURN> to release lock.\n");
	getchar ();

	fl.l_type = F_UNLCK;					/* Tell is to unlock the region */
	errno = 0;
	if (fcntl (fd, F_SETLK, &fl) == -1) {	/* Set the region to unlocked */
		perror ("fcntl()");
		return EXIT_FAILURE;
	}
	printf ("Unlocked.\n");
	close (fd);
	return EXIT_SUCCESS;
}
