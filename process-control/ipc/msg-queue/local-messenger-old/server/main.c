#include <stdio.h>
#include <stdlib.h> // exit
#include <time.h>
#include <mqueue.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h> // usleep

typedef struct chat_user
{
    char *chat_username;
    char *read_queue_name;
    char *write_queue_name;
} chat_user;

typedef struct user_list
{
    bool is_authorized;
    mqd_t read_queue_id;
    mqd_t write_queue_id;
    struct chat_user user;
    struct user_list *next;
} user_list;

typedef struct message
{
    char *msg_sender_name;
    char *msg_text;
    struct tm *msg_time;
} message;

typedef struct message_list
{
    struct message msg;
    struct message_list *next;
} message_list;

typedef struct server_answer
{
    bool is_authorized;
    char *covering_letter;
} server_answer;

int const MSG_LEN_MAX = 256;
int const MESSAGES_LIMIT = 8;
int const SLEEP_TIME = 50000;

/**
 * @brief Send a message to a user
 *
 * @param usr
 * @param msg
 */
void send_message(
    struct chat_user usr,
    struct message *msg);
/**
 * @brief Add the first user to start the messenger
 * Why else start it wihtout users?
 * Also initialize pointers
 *
 * @param read_mq_id
 * @param usr_list
 * @param usr_list_copy
 * @param msg_list
 * @param msg_list_copy
 */
void add_first_user(
    mqd_t input_mq_id,
    struct user_list **usr_list,
    struct user_list **usr_list_copy,
    struct message_list **msg_list,
    struct message_list **msg_list_copy);
/**
 * @brief Handle all add requests from clients
 *
 * @param input_mq_id
 * @param last_user
 * @param last_message
 */
void add_new_users(
    mqd_t input_mq_id,
    struct user_list **last_user,
    struct message_list **last_message);
/**
 * @brief Create a user list element object
 *
 * @param user
 * @return struct user_list*
 */
struct user_list *create_user_list_element(struct chat_user **user);
/**
 * @brief Create a welcome message object
 *
 * @param msg_txt
 * @return struct message_list*
 */
struct message_list *create_welcome_message(char *msg_txt);
/**
 * @brief Receive new messages from users and
 * send these messages to others
 *
 * @param first_user
 * @param last_message
 */
void sync_accounts(
    struct user_list *first_user,
    struct message_list **last_message);
/**
 * @brief Create a new user object from a client's message
 * that was sent to the reception queue
 *
 * @param new_user
 * @param user_data
 */
struct chat_user *create_new_user(char *user_data);

int main(void)
{
    char const RECEPTION_QUEUE_NAME[] = "/reception_queue";

    struct user_list *first_user = NULL;
    struct user_list *last_user = NULL;
    struct message_list *first_message = NULL;
    struct message_list *last_message = NULL;

    struct mq_attr mq_attrs;
    mq_attrs.mq_maxmsg = MESSAGES_LIMIT;
    mq_attrs.mq_msgsize = MSG_LEN_MAX;

    mq_unlink(RECEPTION_QUEUE_NAME);
    mqd_t reception_queue_id = mq_open(RECEPTION_QUEUE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, 0622, &mq_attrs);
    if (-1 == reception_queue_id)
    {
        perror("Reception queue (mq_open)");
        exit(EXIT_FAILURE);
    }

    add_first_user(reception_queue_id, &first_user, &last_user, &first_message, &last_message);

    // while (true)
    // {
    //     add_new_users(reception_queue_id, &last_user, &last_message);
    //     sync_accounts(first_user, &last_message);
    //     usleep(SLEEP_TIME);
    // }
}

void add_first_user(
    mqd_t input_mq_id,
    struct user_list **usr_list,
    struct user_list **usr_list_copy,
    struct message_list **msg_list,
    struct message_list **msg_list_copy)
{
    struct chat_user *new_user = NULL;
    char *new_user_buf = malloc(MSG_LEN_MAX);
    int retval = -1;

    while (NULL == *usr_list)
    {
        if (NULL != new_user)
        {
            free(new_user);
            new_user = NULL;
        }

        retval = mq_receive(input_mq_id, new_user_buf, MSG_LEN_MAX, NULL);
        if (-1 == retval)
        {
            if (EAGAIN != errno)
            {
                perror("Reception mq_receive");
            }
        }
        else
        {
            printf("new_user_buf: %s\n", new_user_buf);
            new_user = create_new_user(new_user_buf);
            if (NULL != new_user)
            {
                *usr_list = create_user_list_element(&new_user);
                *usr_list_copy = *usr_list;
                *msg_list = create_welcome_message(new_user->chat_username);
                *msg_list_copy = *msg_list;
            }
        }

        usleep(SLEEP_TIME);
    }

    free(new_user_buf);
}

struct chat_user *create_new_user(char *user_data)
{
    return NULL;
}

struct user_list *create_user_list_element(struct chat_user **user)
{
    return NULL;
}

struct message_list *create_welcome_message(char *msg_txt)
{
    return NULL;
}

void add_new_users(
    mqd_t input_mq_id,
    struct user_list **last_user,
    struct message_list **last_message)
{
}

void sync_accounts(
    struct user_list *first_user,
    struct message_list **last_message)
{
}
