#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "rio.h"
#include "http_handler.h"
#include "config.h"


static char* method2string(http_method method)
{
    switch (method)
    {
        case GET:
            return "GET";
            break;
        case POST:
            return "POST";
            break;
        default:
            return "GET";
            break;
    }
}

static void http_response_create(int connected_fd, char* payload, size_t payload_len)
{
    char buf[BUFFER_MAXLEN];
    
    int response_len = snprintf(buf, BUFFER_MAXLEN,
        "HTTP/1.1 200 OK\r\n"
        "Content-type: text/plain\r\n"
        "Content-length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s",payload_len, payload);
#if CONN_TEST == 1
    if(g_runtime_debug){
        printf("[INFO]-Sleep 30 seconds\r\n\r\n");
    }
    sleep(30);
#endif

    if(g_runtime_debug){
        printf("---response header---\r\n%s\r\n", buf);
    }
    write_rio(connected_fd, buf, response_len);
} 

static void parse_header(int connected_fd)
{
    char buf[BUFFER_MAXLEN];
    int read_bytes = readline_rio(connected_fd, buf, BUFFER_MAXLEN);
    
    while(read_bytes > 0 && strcmp(buf, "\r\n") != 0){
        if(g_runtime_debug){
            printf("%s", buf);
        }
        readline_rio(connected_fd, buf, BUFFER_MAXLEN);
    }
    printf("\r\n");
}

void http_resquest_parser(int connected_fd)
{
    char buf[BUFFER_MAXLEN];
    char method[METHOD_MAXLEN];
    char uri[URI_MAXLEN];
    char version[VERSION_MAXLEN];
     
    readline_rio(connected_fd, buf, BUFFER_MAXLEN);    
    sscanf(buf, "%s %s %s", method, uri, version);

    if(g_runtime_debug){
        printf("---request header--- \r\n");
        printf("%s  %s  %s\r\n", method, uri, version);
    }
    parse_header(connected_fd);

    char payload[PAYLOAD_MAXLEN] = "Hello world !\r\n\0";
    size_t payload_len = strlen(payload);
    http_response_create(connected_fd, payload, payload_len);
}

void http_request_create(int connected_fd, http_method method, char* host, char* uri)
{
    char buf[BUFFER_MAXLEN];
    char* method_str = method2string(method);

    int request_len = snprintf(buf, BUFFER_MAXLEN, 
        "%s %s HTTP/1.1\r\n"        
        "Host: %s\r\n"
        "\r\n",
        method_str, uri, host
    );

    if(g_runtime_debug){
        printf("---request header---\r\n%s\r\n", buf);
    }
    write_rio(connected_fd, buf, request_len);
}

void http_response_parser(int connected_fd)
{
    char read_buf[BUFFER_MAXLEN];
    int readline_res;
   
    if(g_runtime_debug){
        printf("---server response--- \r\n");
    }

    while(readline_res = readline_rio(connected_fd, read_buf, BUFFER_MAXLEN) > 0){
        if(g_runtime_debug){
            printf("%s", read_buf);
        }
    }

    if(readline_res < 0){
#if DEBUG_LOG == 1
        printf("[ERROR]-Read server response error\r\n");
#endif
    }
    if(g_runtime_debug){
        printf("---server response end--- \r\n");
    }
}