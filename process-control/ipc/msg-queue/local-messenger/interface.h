/**
 * @file interface.h
 * @author TotalOverDose
 * @brief To use the local chat you should send
 * struct user instance to the server as a (char *) via
 * the message queue with RECEPTION_MQ_NAME name
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <time.h>

#define RECEPTION_MQ_NAME "/reception_mqueue"

enum
{
    USRNAME_SIZE_MAX = 256,
    MSG_SIZE_MAX = 1024,
};

/**
 * @brief Send a message to the chat as a string via a write message queue
 * Read other messages via a read message queue
 * mqr_name is a name for the read message queue
 * mqw_name is a name for the write message queue
 */
typedef struct message
{
    char msg_sender[USRNAME_SIZE_MAX];
    char msg_txt[MSG_SIZE_MAX - USRNAME_SIZE_MAX - sizeof(struct tm)];
    struct tm msg_time;
} message;

typedef struct user
{
    char user_name[USRNAME_SIZE_MAX]; // should be unique
    char mqr_name[USRNAME_SIZE_MAX];  // must be unique
    char mqw_name[USRNAME_SIZE_MAX];  // must be unique
} user;
