/**
 * server.c -- a simple multithread server program
 */

#include "interface.h"

#include <stdbool.h>
#include <errno.h>

#include <pthread.h>
#include <fcntl.h>

typedef struct client_sock_info
{
    int cl_sock;
    pthread_t thread_id;
    bool is_busy;
    bool is_connection_finished;
} client_sock_info;

void* server_task(void *client_sock_id)
{
    return NULL;
}

int find_free_sock(struct client_sock_info clients[],
                   int clients_count)
{
    for (int i = 0; i < clients_count; i++)
    {
        if (!clients[i].is_busy)
        {
            return i;
        }
    }

    return -1;
}

int main(void)
{
    enum
    {
        CLIENT_SOCKS_COUNT = 64,
        SLEEP_SEC = 2,
    };

    socklen_t socklen = sizeof(struct sockaddr_un);

    int listener_sock = -1;
    int listener_flags = -1;
    int retval = -1;

    int cur_client = 0;
    struct client_sock_info clients[CLIENT_SOCKS_COUNT];

    struct sockaddr_un server_addr;

    for (int i = 0; i < CLIENT_SOCKS_COUNT; i++)
    {
        clients[i].cl_sock = -1;
        clients[i].thread_id = -1;
        clients[i].is_busy = false;
        clients[i].is_connection_finished = false;
    }

    listener_sock = socket(SOCK_DOMAIN, SOCK_TYPE, 0);
    if (-1 == listener_sock)
    {
        perror("Open a socket");
    }

    listener_flags = fcntl(listener_sock, F_GETFL);
    if (-1 == listener_flags)
    {
        perror("Get listener socket flags");
    }
    else
    {
        retval = fcntl(listener_sock, F_SETFL, listener_flags | O_NONBLOCK);
        if (-1 == retval)
        {
            perror("Set listener socket flags");
        }
    }

    server_addr.sun_family = SOCK_DOMAIN;
    strcpy(server_addr.sun_path, LISTENER_SOCK_PATH);

    unlink(server_addr.sun_path);
    retval = bind(listener_sock, (struct sockaddr*) &server_addr, socklen);
    if (-1 == retval)
    {
        perror("Bind to a socket");
    }

    retval = listen(listener_sock, 5);
    if (-1 == retval)
    {
        perror("Listen to clients");
    }

    while (true)
    {
        if (-1 == cur_client)
        {
            sleep(SLEEP_SEC);
            cur_client = find_free_sock(clients, CLIENT_SOCKS_COUNT);
            continue;
        }
        clients[cur_client].cl_sock = accept(listener_sock, NULL, NULL);
        if (-1 == clients[cur_client].cl_sock && EWOULDBLOCK == errno)
        {
            printf("No clients've sent a connection request yet\n");
            sleep(SLEEP_SEC);
        }
        else if (-1 == clients[cur_client].cl_sock)
        {
            perror("Accept a client");
            sleep(SLEEP_SEC);
        }
        else
        {
            retval = pthread_create(&clients[cur_client].thread_id,
            NULL, server_task, &clients[cur_client].cl_sock);
            if (-1 == retval)
            {
                perror("Create a thread for a client");
            }
            else
            {
                clients[cur_client].is_busy = true;
                cur_client = find_free_sock(clients, CLIENT_SOCKS_COUNT);
            }
        }

        for (int i = 0; i < CLIENT_SOCKS_COUNT; i++)
        {
            if (clients[i].is_busy && clients[i].is_connection_finished)
            {
                retval = pthread_join(clients[i].thread_id, NULL);
                if (0 == retval)
                {
                    clients[i].is_busy = false;
                    clients[i].is_connection_finished = false;
                    close(clients[i].cl_sock);
                }
            }
        }
    }

    close(listener_sock);
    unlink(LISTENER_SOCK_PATH);
}
