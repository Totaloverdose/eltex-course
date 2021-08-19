#include "../interface.h"

#include <stdio.h>
#include <unistd.h> // close
#include <stdlib.h> // malloc, free
#include <string.h> // strcpy

#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	int server_sock_id = -1;
	int client_sock_id = -1;
	int retval = -1;
	socklen_t socklen = sizeof(struct sockaddr_in);

	char *msg_buf = malloc(MSG_SIZE);

	struct sockaddr_in server_addr, client_addr;

	server_sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_sock_id)
	{
		perror("Create a socket");
	}

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = SERVER_PORT;

	retval = bind(server_sock_id, (struct sockaddr*) &server_addr, socklen);
	if (-1 == retval)
	{
		perror("Bind a server name");
	}

	retval = listen(server_sock_id, 5);
	if (-1 == retval)
	{
		perror("Listen from clients");
	}
	else
	{
		printf("Waiting for clients' connection requests...\n");
	}

	client_sock_id = accept(server_sock_id, (struct sockaddr*) &client_addr,
			&socklen);
	if (-1 == client_sock_id)
	{
		perror("Accept a client");
	}
	else
	{
		printf("The connection request was accepted\n");
	}

	retval = recv(client_sock_id, msg_buf, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Receive a message form a client");
	}
	else
	{
		printf("Received a message: %s\n", msg_buf);

		strcpy(msg_buf, &msg_buf[1]);

		retval = send(client_sock_id, msg_buf, MSG_SIZE, 0);
		if (-1 == retval)
		{
			perror("Send a message to a client");
		}
	}

	close(server_sock_id);
	close(client_sock_id);

	free(msg_buf);
}
