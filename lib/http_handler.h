#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#define BUFFER_MAXLEN 256
#define METHOD_MAXLEN 10
#define URI_MAXLEN 128
#define VERSION_MAXLEN 10
#define PAYLOAD_MAXLEN 512


void request_handle(int connected_fd);

#endif