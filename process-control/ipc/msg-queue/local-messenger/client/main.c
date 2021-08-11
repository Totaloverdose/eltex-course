#include "../interface.h"

#include <stdio.h>
#include <string.h> // strlen, strcat
#include <stdlib.h> // random, srandom
#include <stdbool.h>

#include <mqueue.h>
#include <pthread.h>

void remove_newline(char *string)
{
    int str_len = strlen(string);
    if ('\n' == string[str_len - 1])
    {
        string[str_len - 1] = '\0';
    }
}

void fill_mq_names(
    char username[USRNAME_SIZE_MAX],
    char mqr_name[USRNAME_SIZE_MAX],
    char mqw_name[USRNAME_SIZE_MAX])
{
    long random_num = 0; // Will be XXXXXX
    char random_string[6];

    srandom(time(NULL));
    random_num = random() / 1000000; // = XXXXXX
    sprintf(random_string, "%ld", random_num);

    strcat(mqr_name, username);
    strcat(mqr_name, random_string);
    memcpy(mqw_name, mqr_name, USRNAME_SIZE_MAX);

    strcat(mqr_name, "read");
    strcat(mqw_name, "write");
}

void connect_to_chat(struct user user, mqd_t *rmq_id, mqd_t *wmq_id)
{
    struct mq_attr attrs = {.mq_msgsize = MSG_SIZE_MAX, .mq_maxmsg = 10};

    *rmq_id = mq_open(user.mqr_name, O_CREAT | O_RDONLY, 0644, &attrs);
    if (-1 == *rmq_id)
    {
        perror("Read mq_open");
        exit(EXIT_FAILURE);
    }

    *wmq_id = mq_open(user.mqw_name, O_CREAT | O_WRONLY, 0622, &attrs);
    if (-1 == *wmq_id)
    {
        perror("Write mq_open");
        exit(EXIT_FAILURE);
    }

    mq_send
}

int main(void)
{
    int retval = -1;

    struct user user = {.user_name = "", .mqr_name = "/", .mqw_name = ""};

    mqd_t rmq_id = -1;
    mqd_t wmq_id = -1;

    pthread_t write_thread = 0;

    printf("Enter your username: ");
    while ("" == user.user_name)
    {
        // Subtract 12 for mq characters:
        // '/', "read" or "write" and 6 random digits
        // (1 + (from 4 to 5) + 6)
        fgets(user.user_name, USRNAME_SIZE_MAX - 12, stdin);
        remove_newline(user.user_name);
    }

    fill_mq_names(user.user_name, user.mqr_name, user.mqw_name);
    connect_to_chat(user, &rmq_id, &wmq_id);

    retval = pthead_create(&write_thread, NULL, write_messages, &wmq_id);
    if (0 != retval)
    {
        fprintf(stderr, "pthread_create: %s\n", strerror(retval));
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        read_messages(rmq_id);
    }
}
