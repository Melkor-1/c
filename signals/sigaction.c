/* Here is an equivalent example using sigaction instead of the standard signal */

#include <signal.h>

void termination_handler (int signum) 
{
	struct temp_file *P;

	for (p = temp_file_list; p; p->next) {
		unlink (p->name);
	}
}

int main(void) 
{
	struct sigaction new_action, old_action;

	/* Set up the structure to specify the new action */
	new_action.sa_handler = termination_handler;
	sigemptyset (&new_action.sa_mask);
	new_action.sa_flags = 0;

	sigaction (SIGINT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction (SIGINT, &new_action, NULL);
	}

	sigaction (SIGHUP, &new_action, NULL);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction (SIGHUP, &new_action, NULL);
	}

	sigaction (SIGTERM, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction (SIGTERM, &new_action, NULL);
	}
	
	/* Here is another example. It retrieves information about the current action 
	*  for SIGINT without changing that action */
	struct sigction query_action;

	if (sigaction (SIGINT, NULL, &query_action) < 0) 
		/* sigaction returns -1 in case of error */
	else if (query_action.sa_handler == SIG_DFL) 
		/* SIGINT is handled in the default, fatal manner */
	else if (query_action.sa_handler == SIG_IGN) 
		/* SIGINT is ignored */
	else 
		/* A programmer-defined signal handler is in effect. */

	return EXIT_SUCCESS:
}

