#ifndef INTERFACE

#define INTERFACE

#include <stdio.h>
#include <string.h> // strcpy, strcat (?)
#include <stdlib.h> // malloc, free
#include <unistd.h> // close, sleep

#include <sys/socket.h>
#include <sys/un.h>

char const *LISTENER_SOCK_PATH = "/tmp/listener_sock";
int const SOCK_DOMAIN = AF_LOCAL;
int const SOCK_TYPE = SOCK_STREAM;

#endif
