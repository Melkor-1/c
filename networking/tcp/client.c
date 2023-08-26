#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "8898"

#define _PROGRAM_NAME_ "client"

static void *get_in_addr(struct sockaddr *sa) 
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

static void check(int status, char *arg) 
{
	if (status == -1) {
		perror(arg);
		exit(EXIT_FAILURE);
	}
}
	
static void setup_address(char *node, struct addrinfo *hints, struct addrinfo **servinfo) 
{	
	int status;

	memset(hints, 0x00, sizeof *hints);
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	
	if ((status = getaddrinfo(node, PORT, hints, servinfo)) != 0) {
		fprintf(stderr, "Error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
}

static void usage(void) 
{
	fprintf(stderr, "Usage: %s <hostname>\n", _PROGRAM_NAME_);
	exit(EXIT_FAILURE);
}

static void connect_host(int *sockfd, struct addrinfo **servinfo, struct addrinfo **p) 
{
	errno = 0;

	/* loop through all the results and connect to the first one we can */
	for (*p = *servinfo; *p; *p = (*p)->ai_next) {
		if ((*sockfd = socket((*p)->ai_family,(*p)->ai_socktype, (*p)->ai_protocol)) == -1) {
		perror("socket");
		continue;
		}

		if (connect(*sockfd, (*p)->ai_addr, (*p)->ai_addrlen) == -1) {
			perror("connect");
			close(*sockfd);
			continue;
		}
		break;
	}
}
	
	
int main(int argc, char **argv) 
{
	int sockfd, total_bytes;
	char buf[BUFSIZ];
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
		usage();
	}
	
	setup_address(argv[1], &hints, &servinfo);
	connect_host(&sockfd, &servinfo, &p);
	
	if (!p) {
		freeaddrinfo(servinfo);
		fprintf(stderr, "%s: failed to connect\n", _PROGRAM_NAME_);
		return EXIT_FAILURE;
	}
	
	errno = 0;
	if (!inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s)) {
		perror("inet_ntop");
		return EXIT_FAILURE;
	}
	freeaddrinfo(servinfo);

	memset(buf, 0x00, sizeof buf);
	while ((total_bytes = recv(sockfd, buf, BUFSIZ - 1, 0)) > 0) {
		printf("%s", buf);
	}
	check(total_bytes, "recv");
	close(sockfd);
	return EXIT_SUCCESS;
}
	

	
