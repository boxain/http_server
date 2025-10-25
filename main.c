#define MAXLINE 128
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

#include "lib/open_listen.h"
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

void thread_routine()
{

}

#endif


int main()
{
    int connected_fd;
    struct sockaddr peer_client;
    size_t peer_client_len;
    int pid;

    char client_host[MAXLINE], clietn_port[MAXLINE];

    int listen_fd = open_listenfd();
    if(listen_fd < 0){
        exit(1);
    }

#if USE_THREADING == 0
    signal(SIGCHLD, child_recycle_handler);
#else

#endif

    while(1){
        peer_client_len = sizeof(peer_client);
        connected_fd = accept(listen_fd, &peer_client, &peer_client_len);
        if(connected_fd < 0){
            // If system call be interrup by Signal
            if(connected_fd == EINTR){
                printf("[INFO]-system call accept be interrupt by Singal\r\n");
                continue;
            }else{
                printf("[ERROR]-failed to accept connection of new client\r\n");
                continue;
            }
        }

        getnameinfo(&peer_client, peer_client_len, client_host, MAXLINE, clietn_port, MAXLINE, NI_NUMERICHOST);
        printf("clietn IPv4: %s, Port: %s\r\n", client_host, clietn_port);
        
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
            request_handle(connected_fd);
            close(connected_fd);
            exit(0);
        case -1:
            printf("[ERROR]-create child process failed\r\n");
            break;
        default:
            printf("[INFO]-parent process create child process: %d\r\n", pid);
            close(connected_fd);
            break;
        }
#else



#endif
        
    }
}

/** Resources
 * 
 * GDB:        https://zhuanlan.zhihu.com/p/1919803617872938161
 * Deassembly: https://huc0day.blog.csdn.net/article/details/151019122?spm=1001.2101.3001.6650.4&utm_medium=
 *             distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.
 *             235%5Ev43%5Epc_blog_bottom_relevance_base1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%
 *             7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.235%5Ev43%5Epc_blog_bottom_relevance_base1&utm_relevant_index=9
 * Ubuntu ps command: 
 *             https://blog.csdn.net/weixin_42148809/article/details/135104281
 */