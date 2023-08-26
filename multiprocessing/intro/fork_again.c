#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) 
{
	pid_t pid;
	int rv;
	errno = 0;

	switch(pid = fork()) {
	case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			
	case 0:
	/* Child process */
			printf(" CHILD: This is the child process!\n");
			printf(" CHILD: My PID is %d\n", getpid());
			printf(" CHILD: MY parent's PID is %d\n", getppid());
			printf(" CHILD: Enter my exit status (make it small): ");
			scanf("%d", &rv);
			printf(" CHILD: I'm outta here.\n");
			exit(rv);
	
	default:
	/* Parent process */
			printf("PARENT: This is the parent process.\n");
			printf("PARENT: My PID is %d\n", getpid());
			printf("PARENT: My child's PID is %d\n", pid);
			printf("PARENT: I am now waiting for the child to exit()...\n");
			waitpid(-1, &rv, 0);	/* Or wait(0), if you don't care about its exit code */
			printf("PARENT: My child's exit status is: %d\n", WEXITSTATUS(rv));
			printf("PARENT: I am outta here.\n");
	}
	return EXIT_SUCCESS;
}
