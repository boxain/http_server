#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#define BUFFER_MAXLEN 256
#define METHOD_MAXLEN 10
#define URI_MAXLEN 128
#define VERSION_MAXLEN 10
#define PAYLOAD_MAXLEN 512

typedef enum {
    GET,
    POST
} http_method;

void http_resquest_parser(int connected_fd);
void http_request_create(int connected_fd, http_method method, char* host, char* uri);
void http_response_parser(int connected_fd);

#endif