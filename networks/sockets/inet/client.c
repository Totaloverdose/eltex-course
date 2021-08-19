#include "../interface.h"

#include <stdio.h>
#include <string.h> // strcpy
#include <stdlib.h> // malloc
#include <unistd.h> // close

#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	struct sockaddr_in server_addr;
	char *message = malloc(MSG_SIZE);

	int retval = -1;
	int socket_id = -1;

	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == socket_id)
	{
		perror("Create a socket");
	}

	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = SERVER_PORT;

	strcpy(message, "Hello!");

	retval = connect(socket_id, (const struct sockaddr *) &server_addr,
			sizeof(struct sockaddr_in));
	if (-1 == retval)
	{
		perror("Connect to the server");
	}

	retval = send(socket_id, message, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Send a message to the server");
	}
	else
	{
		printf("A message was sent: %s\n", message);
	}

	retval = recv(socket_id, message, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Receive a message from the server");
	}
	else
	{
		printf("A message was received: %s\n", message);
	}

	close(socket_id);

	free(message);
}
