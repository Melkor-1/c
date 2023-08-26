#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Standard HTTP port */
#define SERVER_PORT 80

#define MAXLINE 4096
#define SA struct sockaddr 

int main(int argc, char **argv) 
{
	int sockfd, n;
	size_t sendbytes;
	struct sockaddr_in servaddr;

    memset (&servaddr, 0x00, sizeof servaddr);
    servaddr = (struct sockaddr_in) {.sin_family = AF_INET, /* IPv4 Internal protocol. */
                                   .sin_port = htons (SERVER_PORT)}; /* host to network - port 80. */
	char sendline[MAXLINE];
	char recvline[MAXLINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server address>\n", argv[0]);
		return EXIT_FAILURE;
	}	
	/* Establishes a stream socket and returns a file descriptor, AF_INET == IPv4 , 0 == TCP */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	}

	/* Converts argv[1] from text to binary form */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		perror("inet_pton()");
		return EXIT_FAILURE;
	}
	
	/* Connects the socket reffered to by the file descriptor sockfd to the
	*  address specified by addr.
	*/
	if (connect(sockfd, (SA *) &servaddr, sizeof servaddr) == -1) {
		perror("connect()");
		return EXIT_FAILURE;
	}

	/* We're connected. Prepare the message. */
	strcpy (sendline, "GET / HTTP/1.1\r\n\r\n");
	sendbytes = strlen(sendline);

	/* Send the request -- making sure you send it all 
	*  This code is a bit fragile, since it bails if only
	*  some of the bytes are sent.
	*  Normally you'd want to retry, unless the return value
	*  was -1.
	*/
	if (write(sockfd, sendline, sendbytes) != sendbytes) {
		perror("write");
		return EXIT_FAILURE;
	}

	memset(recvline, 0x00, MAXLINE);
	
    /* Now read the server's response */
	while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0) {
        printf("%s", recvline);
	}
	if (n < 0) {
		perror("read");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}



