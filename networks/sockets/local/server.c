#include "../interface.h"

#include <stdio.h>

#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
	int addr_size = sizeof(struct sockaddr_un);
	int retval = -1;
	int socket_id = -1;

	char *msg_buf = malloc(MSG_SIZE);

	struct sockaddr_un server_addr =
	{ .sun_family = AF_LOCAL, .sun_path = SOCK_NAME };
	struct sockaddr_un client_addr;

	socket_id = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (-1 == socket_id)
	{
		perror("Create a socket");
	}

	unlink(server_addr.sun_path);
	retval = bind(socket_id, (struct sockaddr*) &server_addr, addr_size);
	if (-1 == retval)
	{
		perror("Bind id to a socket");
	}

	retval = recvfrom(socket_id, msg_buf, MSG_SIZE, 0,
			(struct sockaddr*) &client_addr, &addr_size);
	if (-1 == retval)
	{
		perror("Receive from a client");
	}
	else
	{
		printf("Received a message: %s\n", msg_buf);
	}

	retval = sendto(socket_id, msg_buf, MSG_SIZE, 0,
			(struct sockaddr*) &client_addr, addr_size);
	if (-1 == retval)
	{
		perror("Send to a client");
	}
}
