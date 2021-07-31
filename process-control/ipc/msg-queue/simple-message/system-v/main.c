#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int const FNAME_MAX = 32;
int const PROJECT_ID = 38552;
enum
{
    MSG_SIZE = 1024,
};

int main(void)
{
    typedef struct my_msg
    {
        long m_type;
        char m_text[MSG_SIZE];
    } my_msg;

    char *file_name = malloc(FNAME_MAX);
    int fd = -1;
    key_t ipc_key = -1;
    int mq_id = -1;
    pid_t pid = -1;

    strcpy(file_name, "/tmp/XXXXXX");
    fd = mkstemp(file_name);
    if (-1 == fd)
    {
        perror("mkstemp");
    }
    else
    {
        printf("Created temporary file that's named %s\n", file_name);
    }

    ipc_key = ftok(file_name, PROJECT_ID);
    if (-1 == ipc_key)
    {
        perror("ftok");
    }

    mq_id = msgget(ipc_key, IPC_CREAT | 0600);
    if (-1 == mq_id)
    {
        perror("msgget");
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
    }

    if (0 == pid)
    {
        struct my_msg msg;

        if (-1 == msgrcv(mq_id, &msg, MSG_SIZE, 3, 0))
        {
            perror("msgrcv");
        }
        else
        {
            printf("\nReceived a message: %s\n\n", msg.m_text);
        }

        if (-1 == msgrcv(mq_id, &msg, MSG_SIZE, 0, 0))
        {
            perror("msgrcv (2)");
        }
        else
        {
            printf("\nReceived a message: %s\n\n", msg.m_text);
        }
    }
    else
    {
        struct my_msg msg[2] = {{5, "The first message"}, {3, "The second m"}};
        if (-1 == msgsnd(mq_id, &msg[0], MSG_SIZE, 0))
        {
            perror("msgsnd");
        }
        if (-1 == msgsnd(mq_id, &msg[1], MSG_SIZE, 0))
        {
            perror("msgsnd (2)");
        }

        wait(NULL);

        if (-1 == unlink(file_name))
        {
            perror("unlink");
        }
    }

    if (-1 == close(fd))
    {
        perror("close");
    }
}
