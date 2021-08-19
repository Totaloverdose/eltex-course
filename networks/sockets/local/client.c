#include "../interface.h"

#include <stdio.h>
#include <string.h> // strcpy
#include <stdlib.h> // malloc
#include <unistd.h> // close, unlink

#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
	struct sockaddr_un server_addr, client_addr;
	char *message = malloc(MSG_SIZE);

	int retval = -1;
	int socket_id = -1;

	socket_id = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (-1 == socket_id)
	{
		perror("Create a socket");
	}

	server_addr.sun_family = client_addr.sun_family = AF_LOCAL;
	strcpy(server_addr.sun_path, SOCK_NAME);
	strcpy(client_addr.sun_path, "/tmp/client_sock");
	strcpy(message, "Hello!");

	unlink(client_addr.sun_path);
	retval = bind(socket_id, (struct sockaddr*) &client_addr,
			sizeof(struct sockaddr_un));
	if (-1 == retval)
	{
		perror("Bind a socket to the client");
	}

	retval = connect(socket_id, (const struct sockaddr*) &server_addr,
			sizeof(struct sockaddr_un));
	if (-1 == retval)
	{
		perror("Connect to a server");
	}

	retval = send(socket_id, message, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Send a message to a server");
	}
	else
	{
		printf("A message was sent: %s\n", message);
	}

	retval = recv(socket_id, message, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Receive a message from a server");
	}
	else
	{
		printf("A message was received: %s\n", message);
	}

	close(socket_id);
	unlink(client_addr.sun_path);

	free(message);
}
