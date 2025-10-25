#include "open_listen.h"
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int open_listenfd()
{
    struct addrinfo hints;
    struct addrinfo *list_rp, *rp; 
    char host_buf[HOST_MAXLEN];
    char service_buf[SERVICE_MAXLEN];
    int socket_fd;
    
    int result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;             // IPv4
    hints.ai_flags = AI_PASSIVE;           // Server
    hints.ai_socktype = SOCK_STREAM ;      // TCP

    result = getaddrinfo(NULL, LISTEN_PORT, &hints, &list_rp);
    if(result != 0){
        fprintf(stderr, "[ERROR]-getaddrinfo error: %s\r\n", gai_strerror(result));
        return -1;
    }

    /**
     * getaddrinfo() returns a list of address structures.
     * Try each address until we successfully bind().
     * If socket() or bind() fails, we close the socket and try the next address.
     */
    for(rp = list_rp; rp != NULL ; rp = rp->ai_next){
        getnameinfo(rp->ai_addr, rp->ai_addrlen, host_buf, HOST_MAXLEN, service_buf, SERVICE_MAXLEN, NI_NUMERICHOST);
        printf("IPv4: %s, Port: %s\r\n", host_buf, service_buf);
        
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); 
        if(socket_fd < 0){
            // fprintf(stderr ,"[ERROR]-socket error: %s\r\n", stderr(error));
            continue;
        }

        result = bind(socket_fd, rp->ai_addr, rp->ai_addrlen);
        if(result < 0){
            close(socket_fd);
            continue;
        }else{
            break;
        }    
    }

    freeaddrinfo(list_rp);
    if(rp == NULL){
        fprintf(stderr, "[ERROR]-failed to bind protocol.\r\n");
        return -1;
    }
    
    result = listen(socket_fd, MAX_CONNECTION);
    if(result < 0){
        fprintf(stderr, "[ERROR]-failed to listen protocol.\r\n");
        close(socket_fd);
        return -1;
    }
    printf("[INFO]-create http server successfully.\r\n");
    return socket_fd;
}