#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_NAME_SIZE 256

int main(void) 
{
	char host_name[MAX_NAME_SIZE];

	const uid_t uid = getuid();
	const struct passwd *const pw = getpwuid(uid);
	const int status = gethostname(host_name, sizeof host_name);
	
	printf("%s@%s ~: ", pw->pw_name, host_name);
	return EXIT_SUCCESS;
	
}

