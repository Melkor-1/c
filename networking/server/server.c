#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
 #include <arpa/inet.h>

#define MAXLINE 4096
#define SERVER_PORT 18000
#define SA struct sockaddr

int main(int argc, char **argv) 
{
	int listenfd, connfd, n;
	struct sockaddr_in servaddr = {.sin_family = AF_INET, .sin_addr.s_addr = htonl (INADDR_ANY), .sin_port = htons (SERVER_PORT)};
	char buf[MAXLINE + 1];
	char recvline[MAXLINE + 1];

	/* Established a stream socket and returns its file descriptor, AF_INET == IPv4, 0 == TCP */ 
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		perror(argv[0]);
		return EXIT_FAILURE;
	}

	/* When a socket is established, it exists in a name space (address family
	*  but has no address assigned to it. bind() assigns the address specified
	*  by the second argument to the socket specified by listenfd.
	*/
	if ((bind(listenfd, (SA *) &servaddr, sizeof servaddr)) == -1) {
		perror(argv[0]);
		return EXIT_FAILURE;
	}

	/* listen() marks the socket referred to by listenfd as a passive socket,
	*  that is, as a socket that will be used to accept incoming connection
	*  requests using accept. 
	*/
	if ((listen(listenfd, 10) == -1)) {
		perror(argv[0]);
		return EXIT_FAILURE;
	}
	for (;;) {
		struct sockaddr_in addr;
		socklen_t addr_len;
		char client_address[MAXLINE + 1];
		
        /* accept() blocks until an incoming connection arrives
		*  it returns a "file descriptor" to the connection 
		*/
		printf("Waiting for a connection on port %d...\n", SERVER_PORT);
		if ((connfd = accept(listenfd, (SA *) &addr, &addr_len)) == -1) {
			perror(argv[0]);
			return EXIT_FAILURE;
		}
		/* Takes an address in network (binary) format and convert it to 
		*  presentation format.
		*/
		inet_ntop(AF_INET, &addr, client_address, MAXLINE);
		printf("Client connection: %s", client_address);

		/* zero out the recieve buffer to make sure it ends up null terminated */
		memset(recvline , 0x00, MAXLINE);

		/* Now read the client's message */
		while ((n = read(connfd, recvline, MAXLINE - 1)) > 0) {
			fprintf(stdout, "\n%s\n", recvline);

			/* Hacky way to detect the end of the message */
			if (recvline[n - 1] == '\n') {
				break;
			}
			memset(recvline, 0x00, MAXLINE);
		}
		if (n < 0) {
			perror("read");
			return EXIT_FAILURE;
		}

		/* Now send a response */
		snprintf(buf, sizeof buf, "HTTP/1.0 200 OK\r\n\r\nHello");

		if ((write(connfd, buf, strlen(buf)) == -1)) {
			perror("write");
			return EXIT_FAILURE;
		}
		if (close(connfd) == -1) {
			perror("close");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

