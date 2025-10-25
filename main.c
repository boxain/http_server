#define MAXLINE 128

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

#include "lib/open_listen.h"
#include "lib/http_handler.h"

/**
 * GDB:        https://zhuanlan.zhihu.com/p/1919803617872938161
 * Deassembly: https://huc0day.blog.csdn.net/article/details/151019122?spm=1001.2101.3001.6650.4&utm_medium=
 *             distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.
 *             235%5Ev43%5Epc_blog_bottom_relevance_base1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%
 *             7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.235%5Ev43%5Epc_blog_bottom_relevance_base1&utm_relevant_index=9
 * 
 */

int main()
{
    int connected_fd;
    struct sockaddr peer_client;
    size_t peer_client_len;

    char client_host[MAXLINE], clietn_port[MAXLINE];

    int listen_fd = open_listenfd();
    if(listen_fd < 0){
        exit(1);
    }

    while(1){
        peer_client_len = sizeof(peer_client);
        connected_fd = accept(listen_fd, &peer_client, &peer_client_len);
        getnameinfo(&peer_client, peer_client_len, client_host, MAXLINE, clietn_port, MAXLINE, NI_NUMERICHOST);
        printf("clietn IPv4: %s, Port: %s\r\n", client_host, clietn_port);
        
        // create process to handler connection, but now just directly handle in same process.
        request_handle(connected_fd);
        close(connected_fd);
    }
}