#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    MSG_SIZE = 1024,
};

int main(void)
{
    char mq_name[] = "/my_queue";
    mqd_t mq_id = -1;
    pid_t pid = -1;
    struct mq_attr mq_attrs;

    mq_attrs.mq_maxmsg = 2;
    mq_attrs.mq_msgsize = MSG_SIZE;

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
    }

    if (0 == pid)
    {
        char msg_txt[MSG_SIZE];
        unsigned int msg_prio = -1;
        mq_id = mq_open(mq_name, O_RDONLY | O_CREAT, 0600, &mq_attrs);
        if (-1 == mq_id)
        {
            perror("mq_open (read)");
        }

        if (-1 == mq_receive(mq_id, msg_txt, MSG_SIZE, &msg_prio))
        {
            perror("mq_receive (1)");
        }
        else
        {
            printf("\nReceived a message: %s\n\n", msg_txt);
        }

        if (-1 == mq_receive(mq_id, msg_txt, MSG_SIZE, &msg_prio))
        {
            perror("mq_receive (2)");
        }
        else
        {
            printf("\nReceived a message: %s\n\n", msg_txt);
        }
    }
    else
    {
        char msg_txt[2][MSG_SIZE] = {"The first message text", "The second message"};
        mq_id = mq_open(mq_name, O_WRONLY | O_CREAT, 0600, &mq_attrs);
        if (-1 == mq_id)
        {
            perror("mq_open (write)");
        }

        if (-1 == mq_send(mq_id, msg_txt[0], MSG_SIZE, 11))
        {
            perror("mq_send (1)");
        }

        if (-1 == mq_send(mq_id, msg_txt[1], MSG_SIZE, 10))
        {
            perror("mq_send (2)");
        }

        wait(NULL);

        if (-1 == mq_unlink(mq_name))
        {
            perror("mq_unlink");
        }
    }

    if (-1 == mq_close(mq_id))
    {
        perror("mq_close");
    }
}
