#include "interface.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(void)
{
    mqd_t reception_queue_id = -1;
    int retval = -1;
    struct chat_user user;
    char *my_username = "Biba";
    int username_size = strlen(my_username) + 1;

    reception_queue_id = mq_open(RECEPTION_QUEUE_NAME, O_WRONLY);
    if (-1 == reception_queue_id)
    {
        perror("Reception mq_open");
    }

    user.chat_username = malloc(username_size);
    strcpy(user.chat_username, my_username);

    user.read_queue_name = malloc(username_size + 5);
    sprintf(user.read_queue_name, "/");
    strcat(user.read_queue_name, my_username);
    strcat(user.read_queue_name, "read");

    user.write_queue_name = malloc(username_size + 6);
    sprintf(user.write_queue_name, "/");
    strcat(user.write_queue_name, my_username);
    strcat(user.write_queue_name, "write");

    retval = mq_send(reception_queue_id, (char *)&user, MESSAGE_LENGTH_MAX, 10);
    if (-1 == retval)
    {
        perror("Reception mq_send");
    }
}
