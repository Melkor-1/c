/*
*  showip.c -- show IP addresses for a host given on the command line 	
*/

#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif

#define _POSIX_C_SOURCE 200819L
#define _XOPEN_SOURCE   700

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PROGRAM_NAME "showip"

static void usage_err(void) 
{
	fprintf(stderr, "Usage: %s <hostname>\n", PROGRAM_NAME);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		usage_err();
	}

	int status;
	struct addrinfo hints = {.ai_family = AF_UNSPEC, /* IP-agnostic. */
                             .ai_socktype = SOCK_STREAM}; /* TCP stream socket. */
	struct addrinfo *servinfo; 			/* Will point to the results */
	struct addrinfo *p;
	char ipstr[INET6_ADDRSTRLEN];

	if (status = getaddrinfo(argv[1], 0, &hints, &servinfo)) {
		fprintf(stderr, "Error: %s\n", gai_strerror(status));
		return EXIT_FAILURE;
	}
	
	printf("IP addresses for %s:\n\n", argv[1]);

	for (p = servinfo; p; p = p->ai_next) {
		void *addr;
		const char *ipver;

		/* get the pointer to the address itself,
		*  different fields in IPv4 and IPv6:
		*/
		if (p->ai_family == AF_INET) { /* IPv4 */
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { 	/* IPv6 */
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		/* Converts the IP in human-readable form */
		errno = 0;
		if (!inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr)) {
			perror(argv[0]);
			return EXIT_FAILURE;
		}
		printf("  %s: %s\n", ipver, ipstr);
	}
	freeaddrinfo(servinfo); 
	return EXIT_SUCCESS;
}

