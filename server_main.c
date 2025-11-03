#define MAXLINE       128
#define USE_THREADING 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <pthread.h>

#include "lib/open_listen.h"
#include "lib/open_client.h"
#include "lib/http_handler.h"


#if USE_THREADING == 0

#include <sys/types.h>
#include <sys/wait.h>

void child_recycle_handler(int sig)
{
    int status;
    int pid;       
    while(pid = waitpid(-1, &status, 0) > 0){  // be blocked until one of the childe process return
        printf("[INFO]-recycle child process: %d, the status: %d\r\n", pid, status);
    }
    return;
}

#else

void thread_routine(void* arg)
{
    printf("[INFO]-enter thread]\r\n");
    int connected_fd = *(int*)arg;
    free(arg);

    pthread_detach(pthread_self());
    request_handle(connected_fd);
    close(connected_fd);
    printf("[INFO]-completed thread, recycle by kernel\r\n");
}

#endif


int main()
{
    int connected_fd;
    struct sockaddr peer_client;
    size_t peer_client_len;
    int res;
    char client_host[MAXLINE], clietn_port[MAXLINE];
#if USE_THREADING == 0
    int pid;
#else
    unsigned long tid;
#endif

    
    int listen_fd = open_listenfd();
    if(listen_fd < 0){
        exit(1);
    }

#if USE_THREADING == 0
    signal(SIGCHLD, child_recycle_handler);
#endif

    while(1){
        peer_client_len = sizeof(peer_client);
        connected_fd = accept(listen_fd, &peer_client, &peer_client_len);
        if(connected_fd < 0){
            // If system call be interrup by Signal
            if(errno == EINTR){
#if DEBUG_LOG == 1
                printf("[INFO]-System call 'accept' be interrupt by Singal\r\n");
#endif
                continue;
            }else{
#if DEBUG_LOG == 1
                printf("[ERROR]-failed to accept connection of new client, error: %d\r\n", errno);
#endif
                break;
            }
        }

        res = getnameinfo(&peer_client, peer_client_len, client_host, MAXLINE, clietn_port, MAXLINE, NI_NUMERICHOST);
#if DEBUG_LOG == 1
        if(res == 0){
            printf("[INFO]-client IPv4: %s, Port: %s\r\n", client_host, clietn_port);
        }else{
            printf("[ERROR]-failed to parse client info, the reason: %s\r\n", gai_strerror(res));
        }
#endif
        
#if USE_THREADING == 0
        // https://man7.org/linux/man-pages/man2/fork.2.html
        pid = fork();
        switch (pid)
        {
        case 0:
            /**
             * Child process
             */
            close(listen_fd);
            http_resquest_parser(connected_fd);
            close(connected_fd);
            exit(0);
        case -1:
#if DEBUG_LOG == 1
            printf("[ERROR]-create child process failed\r\n");
#endif
            break;
        default:
            printf("[INFO]-parent process create child process: %d\r\n\r\n", pid);
            close(connected_fd);
            break;
        }
#else
        // https://man7.org/linux/man-pages/man3/pthread_create.3.html
        int* thread_connected_fd = malloc(sizeof(int));
        *thread_connected_fd = connected_fd;

        pthread_create(&tid, NULL, thread_routine, (void* )thread_connected_fd);
#if DEBUG_LOG == 1
        printf("[INFO]-create thread: %d sucessfully\r\n", tid);
#endif
        
#endif
        
    }
}