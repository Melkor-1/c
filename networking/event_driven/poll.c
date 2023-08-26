#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

#define SIZEOF(x) ((sizeof x) / sizeof (x[0])) 

int main(void) 
{
	struct pollfd pfds[1];	/* More if you want to monitor more */

	pfds[0].fd = STDIN_FILENO;	/* Standard input */	
	pfds[0].events = POLLIN;		/* Alert me when data is ready to recv() */

	printf("Hit RETURN or wait 2.5 seconds for timeout.\n");
	int num_events = poll(pfds, SIZEOF(pfds), 2500); /* 2.5 second timeout */

	/* NB that poll() returns the number of elements in the pfds arra for which
	*  events have occured. It doesn't tell you which elements in the array (you
	*  still have to scan for that), but it does tell you how many entries have a 
	*  non-zero revents field (so you can stop scanning after you find that many).
	*
	*  The revents member of pollfd is a bitmap of events that occurred. It has 
	*  two fields, namely POLLIN and POLLOUT. One can check which field is set
	*  by bitwise-anding with the revents member. 
	*/
	if (!num_events) {
		printf("Poll timed out.\n");
	} else {
		int pollin_happened = pfds[0].revents & POLLIN;	
		if (pollin_happened) {
			printf("File descriptor %d is ready to read.\n", pfds[0].fd);
		} else {
			printf("Unexpected event occured: %d\n", pfds[0].revents);
		}
	}
	return EXIT_SUCCESS;
}
