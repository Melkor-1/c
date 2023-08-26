#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#define SERVERPORT 8979
#define BUFSIZE 4096
#define BACKLOG 1000
#define SOCKETERROR (-1) 
#define SA struct sockaddr


static void check(int exp, const char *msg) 
{
	if (exp == SOCKETERROR) {
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

static int setup_server(void) 
{
	int server_socket;
	size_t addr_size;
	struct sockaddr_in server_addr;

	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVERPORT);

	check(bind(server_socket, (SA *)&server_addr, sizeof server_addr), "bind");

	check(listen(server_socket, BACKLOG), "listen");
	return server_socket;
}

static int accept_connection(int server_socket) 
{
		int client_socket;
		size_t addr_size;
		struct sockaddr_in client_addr;

		addr_size = sizeof (struct sockaddr_in);
		check((client_socket = accept(server_socket, (SA *) &client_addr, (socklen_t *) &addr_size)), "accept");
		puts("Connected");
	
		return client_socket;
}

static void handle_connection(int client_socket) 
{
	char buffer[BUFSIZE] = {0};
	size_t bytes_read;
	size_t msg_size = 0;
	
	while((bytes_read = read(client_socket, buffer + msg_size, sizeof buffer - msg_size -1)) > 0) {
		msg_size += bytes_read;
		if (msg_size > BUFSIZE - 1 || buffer[msg_size - 1] == '\n') {
		   		break;
		}
		check(bytes_read, "read");
		buffer[bytes_read - 1] = '\0';
	}
	printf("REQUEST: %s\n", buffer);
	buffer[strcspn(buffer, "\r\n")] = '\0';

	/* Validity check */
	char *abs_path = realpath(buffer, 0);
	if (!abs_path) {
		perror("realpath");
		close(client_socket);
		return;
	} 
	
	int fd = open(abs_path, O_RDONLY);
	if (fd == -1) {
		perror("open");
		free(abs_path);
		close(client_socket);
		return;
	}
	
	while ((bytes_read = read(fd, buffer, BUFSIZE)) > 0) {
		printf("Sending %zu bytes\n", bytes_read);
		if (write(client_socket, buffer, bytes_read) != bytes_read) {
			perror("write");
			goto CLEAN_UP;
			return;
		}
	}
	CLEAN_UP:
	free(abs_path);
	close(client_socket);
	close(fd); 
	printf("Closing connection\n");
}

int main(int argc, char **argv) 
{
	int server_socket = setup_server();
	while (true) {
		printf("Waiting for connections...\n");
	
		/* Wait for, and eventually accept an incoming connection */
		int client_socket = accept_connection(server_socket);

		handle_connection(client_socket);
	}

	check(close(server_socket), "close");
	return EXIT_FAILURE;
}

