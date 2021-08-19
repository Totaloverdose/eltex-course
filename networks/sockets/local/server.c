#include "../interface.h"

#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strcpy
#include <unistd.h> // unlink, close
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>

int main(void)
{
	socklen_t addr_size = sizeof(struct sockaddr_un);
	int retval = -1;
	int socket_id = -1;

	char *msg_buf = malloc(MSG_SIZE);

	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;

	socket_id = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (-1 == socket_id)
	{
		perror("Create a socket");
	}

	server_addr.sun_family = AF_LOCAL;
	strcpy(server_addr.sun_path, SOCK_NAME);

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

		msg_buf[2] = 'e';

		retval = sendto(socket_id, msg_buf, MSG_SIZE, 0,
				(struct sockaddr*) &client_addr, addr_size);
		if (-1 == retval)
		{
			int cur_errno = errno;
			printf("Error number: %d\n", cur_errno);
			perror("Send to a client");
		}
	}

	close(socket_id);
	unlink(server_addr.sun_path);

	free(msg_buf);
}
