/* Opens up a raw socket and listens for packets. While this program will capture packets, 
*  it isn't reliable and will miss some packets, especially when there is a lot of traffic.
*  Also it only captures TCP packets -- to capture UDP or ICMP packets, additional raw 
*  sockets need to be opened for each.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#include "hex_dump.h"

int main(void) 
{
	int sockfd;
	unsigned char buffer[BUFSIZ];

	errno = 0;
	if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	while (true) {
		ssize_t recv_len = recv(sockfd, buffer, BUFSIZ, 0);
		printf("Got a %zd byte packet\n", recv_len);
		dump(buffer, recv_len);	
		
		if (recv_len < 0) {
			perror("recv");
			continue;
		}
	}
	close(sockfd);
	return EXIT_SUCCESS;
}
