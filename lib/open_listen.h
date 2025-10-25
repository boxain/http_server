#ifndef OPEN_LISTEN_H
#define OPEN_LISTEN_H

#define _POSIX_C_SOURCE    200809L
#define HOST_MAXLEN        50
#define SERVICE_MAXLEN     50
#define LISTEN_PORT        "8000"
#define MAX_CONNECTION     1024

int open_listenfd();

#endif