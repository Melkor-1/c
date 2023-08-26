#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define PORT "8898"		/* The port users will be connecting to */
#define BACKLOG 10 		/* Number of pending connections queue will hold */

static void sig_handler(int sig) 
{
	/* waitpid() might overwrite errno, so we save and restore it */
	int old_errno = errno;
	
	while (waitpid(-1, 0, WNOHANG) > 0);
	
	errno = old_errno;
}

static void check(int status, char *arg) 
{
	if (status == -1) {
		perror(arg);
		exit(EXIT_FAILURE);
	}
}

static void *get_in_addr(struct sockaddr *sa) 
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

static void setup_addr(struct addrinfo *hints, struct addrinfo **servinfo) 
{
	int status;

	memset(hints, 0x00, sizeof *hints);
	hints->ai_family = AF_UNSPEC;	/* IPv-agnostic */
	hints->ai_socktype = SOCK_STREAM; /* stream socket */
	hints->ai_flags = AI_PASSIVE; 	/* my IP */

	if ((status = getaddrinfo(0, PORT, hints, servinfo)) != 0) {
		fprintf(stderr, "Error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
}

static int setup_socket(struct addrinfo **servinfo) 
{	
	int server_fd; 
	int yes = 1;	
	struct addrinfo *p;

	/* loop through all the results and bind to the first one we can */
	for (p = *servinfo; p != 0; p = p->ai_next) {
		if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			continue;
		}

		if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("bind");
			continue;
		}
		break;
	}
	
	if (!p) {
		fprintf(stderr, "Error: failed to connect.\n");
		freeaddrinfo(*servinfo);
		return EXIT_FAILURE;
	}

	freeaddrinfo(*servinfo);
	check(listen(server_fd, BACKLOG), "listen");
	return server_fd;
}

static int accept_connections(int server_fd)
{	
	int client_fd;
	struct sockaddr_storage client_addr;
	socklen_t sin_size = sizeof client_addr;
	char client_ip[INET6_ADDRSTRLEN];

	check(client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sin_size), "accept");
		
	errno = 0;
	if (!inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), client_ip, sizeof client_ip)) {
		perror("inet pton");
		exit(EXIT_FAILURE);
	}
	return client_fd;
}

int main(void) 
{
	int server_fd, client_fd; 	/* listen on server_fd, connect on client_fd */
	struct addrinfo hints, *servinfo;
	struct sigaction sa;

	setup_addr(&hints, &servinfo);
	server_fd = setup_socket(&servinfo);

	printf("Waiting for connections on port %s...\n", PORT);
	
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	check (sigaction(SIGCHLD, &sa, 0), "sigaction");

	while (true) {
		client_fd = accept_connections(server_fd);

		if (!fork()) { /* child process */
			close(server_fd);/* do not need the listener */
			if (send(client_fd, "Hello, world!\n", 14, 0) == -1) {
				perror("send");
			}
			printf("Closing connection.\n");
			close(client_fd);
			return EXIT_SUCCESS;
		} 
		close(client_fd);
	}
	return EXIT_SUCCESS;
}
