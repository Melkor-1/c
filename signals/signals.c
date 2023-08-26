/* 
*	Here is a simple example of setting up a handler to delete temporary files when 
* 	certain fatal signals happen:
* 	SIGINT 
*/

#include <signal.h>

void termination_handler (int signum) 
{
	struct temp_file *p;

	for (p = temp_file_list; p; p = p->next) {
		unlink (p->name);
	}
}

/* SIG_IGN specifies that the signal should be ignored. You cannot ignore the SIGKILL or SIGSTOP 
*  signals at all. Note that if a given signal was previously set to be ignored, this code avoids
*  altering that setting.
*/
int main(void)
{
	if (signal (SIGINT, termination_handler) == SIG_IGN) {	
		signal (SIGINT, SIG_IGN);							
	} else if (signal (SIGHUP, termination_handler) == SIG_IGN) {
		signal (SIGHUP, SIG_IGN);
	} else if (signal (SIGTERM, termination_handler) == SIG_IGN) {
		signal (SIGTERM, SIG_IGN);
	}
	return EXIT_FAILURE;
}
