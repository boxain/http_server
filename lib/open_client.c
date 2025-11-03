#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "open_client.h"
#include "http_handler.h"


void open_clientfd(void* arg)
{
    int* thread_identify = (int*)arg;
    printf("[INFO]-Thread identify: %d\r\n", *thread_identify);
    // pthread_detach(pthread_self());

    int result;
    struct addrinfo hints;
    struct addrinfo *list_rp, *rp;
    char host_buf[HOST_MAXLEN];
    char service_buf[SERVICE_MAXLEN];
    int socket_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;         // IPv4
    hints.ai_socktype = SOCK_STREAM;   // TCP

    result = getaddrinfo(CONNECTION_HOST, CONNECTION_PORT, &hints, &list_rp);
    if(result != 0){
#if DEBUG_LOG == 1
        fprintf(stderr, "[ERROR]-getaddrinfo error: %s\r\n", gai_strerror(result));
#endif
        return;
    }


    /**
     * getaddrinfo() returns a list of address structures.
     * Try each address until we successfully connect().
     * If socket() or connect() fails, we close the socket and try the next address.
     */
    for(rp = list_rp; rp != NULL ; rp=rp->ai_next){
        result = getnameinfo(rp->ai_addr, rp->ai_addrlen, host_buf, HOST_MAXLEN, service_buf, SERVICE_MAXLEN, NI_NUMERICHOST);
#if DEBUG_LOG == 1
        if(result == 0){
            printf("[INFO]-Server listening for IPv4: %s, Port: %s\r\n", host_buf, service_buf);
        }else{
            printf("[ERROR]-failed to parse client info, the reason: %s\r\n", gai_strerror(result));
        }
#endif
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket < 0){
#if DEBUG_LOG == 1
            printf("[ERROR]-socket descriptor create error: %d\r\n", socket_fd);
#endif
            continue;
        }

        result = connect(socket_fd, rp->ai_addr, rp->ai_addrlen);
        if(result < 0){
            close(socket_fd);
            continue;
        }else{
            break;
        }

    }
    
    freeaddrinfo(list_rp);
    if(rp == NULL){
#if DEBUG_LOG == 1
        fprintf(stderr, "[ERROR]-failed to bind protocol.\r\n");
#endif
    }

    // create http request 
    http_request_create(socket_fd, GET, CONNECTION_HOST, "/");
    http_response_parser(socket_fd);

    close(socket_fd);
    printf("[INFO]-Thread: %d request completed\r\n", *thread_identify);
    free(thread_identify);
    return;
}