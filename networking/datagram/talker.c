/* 
*	talker.c --- a datagram "client"
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_PORT "4950"
#define PROGRAM_NAME "talker"

static void init_addr(char *host, struct addrinfo *hints, struct addrinfo **servinfo) 
{
	int status;

	memset(hints, 0x00, sizeof *hints);
	hints->ai_family = PF_INET6;
	hints->ai_socktype = SOCK_DGRAM;

	if ((status = getaddrinfo(host, SERVER_PORT, hints, servinfo)) != 0) {
		fprintf(stderr, "%s: %s\n", PROGRAM_NAME, gai_strerror(status));
		exit(EXIT_FAILURE);
	}
}

static int init_socket(struct addrinfo **servinfo, struct addrinfo **p) 
{
	int sockfd;
	
	errno = 0;
	for (*p = *servinfo; *p; *p = (*p)->ai_next) {
		if ((sockfd = socket((*p)->ai_family, (*p)->ai_socktype, (*p)->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}
		break;
	}
	if (!(*p)) {
		perror("socket");
		return -1;
	}
	return sockfd;
}

int main(int argc, char **argv) 
{
	if (argc != 3) {
		fprintf(stderr, "Usage; %s <hostname> <message>\n", PROGRAM_NAME);
		return EXIT_FAILURE;
	}

	int sockfd;
	struct addrinfo hints, *servinfo, *p;

	init_addr(argv[1], &hints, &servinfo);
	if ((sockfd = init_socket(&servinfo, &p)) == -1) {
		return EXIT_FAILURE;
	}
	
	int total_bytes;
	if ((total_bytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, 
									p->ai_addr, p->ai_addrlen)) == -1) {
		perror("sendto");
		return EXIT_FAILURE;
	}
	freeaddrinfo(servinfo);
	close(sockfd);

	return EXIT_SUCCESS;
}

