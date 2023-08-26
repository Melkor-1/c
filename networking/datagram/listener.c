/* 
*	listener.c -- a datagram sockets "server"
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>

#define PORT "4950"
#define PROGRAM_NAME "listener"

/* Get sockaddr, IPv4 or IPV6 */
static void *get_in_addr(struct sockaddr *sa) 
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	} 
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

static void init_addr(struct addrinfo *hints, struct addrinfo **servinfo) 
{
	int status; 

	memset(hints, 0x00, sizeof *hints);
	hints->ai_family = AF_INET;	
	hints->ai_socktype = SOCK_DGRAM;
	hints->ai_flags = AI_PASSIVE;	/* my ip */

	if ((status = getaddrinfo(0, PORT, hints, servinfo)) != 0) {
		fprintf(stderr, "Error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
}

static int setup_socket(struct addrinfo **servinfo) 
{
	struct addrinfo *p;
	int sockfd;

	errno = 0;
	for (p = *servinfo; p; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		}

		if ((bind(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
			close(sockfd);
			perror("bind");
			continue;
		}
		break;
	}
	if (!p) {
		fprintf(stderr, "%s: Failed to bind.\n", PROGRAM_NAME);
		exit(EXIT_FAILURE);
	}
	return sockfd;
}	

static int accept_connection(int sockfd) 
{	
	char packet[4096];
	char client_ip[INET6_ADDRSTRLEN];
	struct sockaddr_storage client_addr;
	socklen_t addr_len = sizeof client_addr;	
	int total_bytes;

	errno = 0;
	if ((total_bytes = recvfrom(sockfd, packet, BUFSIZ - 1, 0, (struct sockaddr *)&client_addr,
									&addr_len)) == -1) {
		perror("recvfrom");
		return 0;
	}
		
	errno = 0;
	if (!(inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr), 
					client_ip, sizeof client_ip))) {
		perror("inet_ntop");
		return 0;
	}

	printf("%s: Got packet from %s\n", PROGRAM_NAME, client_ip);
	printf("%s: packet is %d bytes long.\n", PROGRAM_NAME, total_bytes);
	packet[total_bytes] = '\0';
	printf("%s: packet contains \"%s\"\n", PROGRAM_NAME, packet);
	
	return 1;
}

int main(void) 
{
	int sockfd;
	struct addrinfo hints, *servinfo;
	
	init_addr(&hints, &servinfo);
	sockfd = setup_socket(&servinfo);

	freeaddrinfo(servinfo);
	
	while (true) {
		printf("Waiting for packets on port %s...\n", PORT);
		 if (!accept_connection(sockfd)) {
		 	break;
		}
	}
	close(sockfd);
	return EXIT_SUCCESS;
}


