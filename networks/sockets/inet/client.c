#include "../interface.h"

#include <stdio.h>
#include <string.h> // strcpy
#include <stdlib.h> // malloc

#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
	struct sockaddr_in server_addr;
	char *message = malloc(MSG_SIZE);

	int retval = -1;
	int socket_id = -1;

	socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == socket_id)
	{
		perror("Create a socket");
	}

	strcpy(message, "Hello!");

	retval = connect(socket_id, (const struct sockaddr *) &server_addr,
			sizeof(struct sockaddr_in));
	if (-1 == retval)
	{
		perror("Connect to a server");
	}

	retval = send(socket_id, message, MSG_SIZE, 0);
	if (-1 == retval)
	{
		perror("Send a message to a server");
	}
}
