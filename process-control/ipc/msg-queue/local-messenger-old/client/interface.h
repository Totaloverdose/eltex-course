/**
 * @file interface.h
 * @author Infernus
 * @brief This interface describes how a client can interact with the chat server
 * Create a chat_user structure and send it to the server through the reception queue
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>
#include <mqueue.h>
#include <stdbool.h>

#define RECEPTION_QUEUE_NAME "/reception_queue"
#define MESSAGE_LENGTH_MAX 256

/**
 * @brief You should see if your queue names are unique
 * yourself
 *
 */
typedef struct chat_user
{
    char *chat_username;
    char *read_queue_name;
    char *write_queue_name;
} chat_user;

typedef struct message
{
    char *msg_sender_name;
    char *msg_text;
    struct tm *msg_time;
} message;

/**
 * @brief The server may deny your authorization trial
 * if you send an existing username or if this username isn't
 * allowed. Then you should send a new username (just a string) to the server
 * through the write queue to get access to the chat
 */
typedef struct server_answer
{
    bool is_authorized;
    char *covering_letter;
} server_answer;

/**
 * @brief You should send a chat_user structure
 * instanse through the reception message queue
 *
 * @param usr
 * @param reception_mq_id
 */
void send_message(struct chat_user *usr, mqd_t reception_mq_id);
/**
 * @brief Send a new username to the server (only if the client isn't authorized)
 *
 * @param username // a new username string
 * @param write_mq_id // the write_queue_name queue id
 */
void send_username(char *username, mqd_t write_mq_id);
