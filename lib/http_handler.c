#include <stdio.h>
#include <string.h>

#include "rio.h"
#include "http_handler.h"

static void response_handle(int connected_fd, char* payload, size_t payload_len)
{
    char buf[BUFFER_MAXLEN];
    
    int response_len = snprintf(buf, BUFFER_MAXLEN,
        "HTTP/1.1 200 OK\r\n"
        "Content-type: text/plain\r\n"
        "Content-length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s",payload_len, payload);
    
    printf("---response header---\r\n%s\r\n", buf);
    write_rio(connected_fd, buf, response_len);
} 

static void parse_header(int connected_fd)
{
    char buf[BUFFER_MAXLEN];
    readline_rio(connected_fd, buf, BUFFER_MAXLEN);
    while(strcmp(buf, "\r\n") != 0){
        printf("%s", buf);
        readline_rio(connected_fd, buf, BUFFER_MAXLEN);
    }
    printf("\r\n");
}

void request_handle(int connected_fd)
{
    char buf[BUFFER_MAXLEN];
    char method[METHOD_MAXLEN];
    char uri[URI_MAXLEN];
    char version[VERSION_MAXLEN];
     
    readline_rio(connected_fd, buf, BUFFER_MAXLEN);    
    sscanf(buf, "%s %s %s", method, uri, version);
    
    printf("---request header--- \r\n");
    printf("%s %s %s", method, uri, version);
    parse_header(connected_fd);

    char payload[PAYLOAD_MAXLEN] = "Hello world !\r\n\0";
    size_t payload_len = strlen(payload);
    response_handle(connected_fd, payload, payload_len);
}