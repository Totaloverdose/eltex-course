#include "../interface.h"

#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <stdlib.h> // exit
#include <unistd.h> // usleep
#include <stdbool.h>
#include <errno.h>

typedef struct user_list
{
    mqd_t rmq_id;
    mqd_t wmq_id;
    struct user user;
    struct msg_list *msg_to_send;
    struct user_list *next;
} user_list;

typedef struct msg_list
{
    struct message msg;
    struct msg_list *next;
} msg_list;

/**
 * @brief Add new users to the end of the user list
 *
 * @param last_user
 * @param first_msg // If NULL, then
 * @param last_msg
 */
void add_new_users(
    mqd_t mq_id,
    struct user_list **last_user,
    struct msg_list *first_msg,
    struct msg_list **last_msg);
/**
 * @brief Create a new user object from the usr_buf
 *
 * @param new_user // Should be an empty object or data loss is possible
 * @param usr_buf
 */
void create_new_user(struct user **new_user, char *usr_buf);
/**
 * @brief Create a new user list element object
 * from the user struct
 *
 * @param list_elem // Should be an empty object or data loss is possible
 * @param user
 */
void create_new_user_list_element(struct user_list **list_elem, struct user *user);
/**
 * @brief Receive new messages from the users
 * and send them them (arathemthem)
 *
 * @param first_user
 * @param last_msg
 */
void sync_messages(struct user_list *first_user, struct msg_list **last_msg);

int const SLEEP_MICROSEC = 50000;

int main()
{
    int const MESSAGES_MAX = 10;

    mqd_t reception_mq_id = -1;
    struct mq_attr mq_attrs = {.mq_maxmsg = MESSAGES_MAX, .mq_msgsize = MSG_SIZE_MAX};

    struct user_list *first_user = NULL;
    struct user_list *last_user = NULL;
    struct msg_list *first_msg = NULL;
    struct msg_list *last_msg = NULL;

    mq_unlink(RECEPTION_MQ_NAME);
    reception_mq_id =
        mq_open(RECEPTION_MQ_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, 0622, &mq_attrs);
    if (-1 == reception_mq_id)
    {
        perror("Reception mq_open");
        exit(EXIT_FAILURE);
    }

    while (NULL == first_user)
    {
        add_new_users(reception_mq_id, &last_user, NULL, &last_msg);
        first_user = last_user;
        first_msg = last_msg;
    }

    while (true)
    {
        add_new_users(reception_mq_id, &last_user, first_msg, &last_msg);
        sync_messages(first_user, &last_msg);
    }
}

void add_new_users(
    mqd_t mq_id,
    struct user_list **last_user,
    struct msg_list *first_msg,
    struct msg_list **last_msg)
{
    struct user *new_user = NULL;
    struct user_list *new_user_list_elem = NULL;
    char *usr_buf = malloc(MSG_SIZE_MAX);
    int retval = -1;

    while (true)
    {
        retval = mq_receive(mq_id, usr_buf, MSG_SIZE_MAX, NULL);
        if (-1 == retval && EAGAIN == errno)
        {
            break;
        }
        else if (-1 == retval)
        {
            perror("Reception mq_receive");
        }

        create_new_user(&new_user, usr_buf);
        create_new_user_list_element(&new_user_list_elem, new_user);

        if (NULL != new_user_list_elem)
        {
            if (NULL != *last_user)
            {
                (*last_user)->next = new_user_list_elem;
            }
            else
            {
                *last_user = new_user_list_elem;
            }
        }

        usleep(SLEEP_MICROSEC);
    }

    free(usr_buf);
}

void create_new_user(struct user **new_user, char *usr_buf)
{
    if (NULL == usr_buf)
    {
        return;
    }

    char *user_name = ((struct user *)usr_buf)->user_name;

    printf("Username: %s\n", user_name);
}

void create_new_user_list_element(struct user_list **list_elem, struct user *user)
{
}

void sync_messages(struct user_list *first_user, struct msg_list **last_msg)
{
}
