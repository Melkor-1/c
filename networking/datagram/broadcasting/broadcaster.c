/* 
*  broadcaster.c -- a datagram client that broadcasts.
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 4095

#define PROGRAM_NAME "broadcaster"

int main(int argc, char **argv) 
{
	int sockfd;
	struct sockaddr_in slave_addr;
	struct hostent *he;
	int num_bytes = 0;
	int broadcast = 1;

	if (argc != 3) {
		fprintf(stderr, "%s: hostname message.\n", PROGRAM_NAME);
		return EXIT_FAILURE;
	}
	
	errno = 0;
	if (!(he = gethostbyname(argv[1]))) {
		perror("gethostbyname");
		return EXIT_FAILURE;
	}
	
	errno = 0;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	/* This call is what allows broadcast messages to be sent:
	*/
	errno = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
		perror("setsockopt");
		return EXIT_FAILURE;
	}
	
	slave_addr.sin_family = AF_INET;
	slave_addr.sin_port = htons(SERVER_PORT);
	slave_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(slave_addr.sin_zero, 0x00, sizeof slave_addr);

	errno = 0;
	num_bytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
				(struct sockaddr *) &slave_addr, sizeof slave_addr);
	if (num_bytes == -1) {
		perror("sendto");
		return EXIT_FAILURE;
	}
	
	printf("sent %d bytes to %s\n", num_bytes, inet_ntoa(slave_addr.sin_addr));
	close(sockfd);
	return EXIT_FAILURE;
}
