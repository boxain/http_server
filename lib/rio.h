#ifndef RIO_H
#define RIO_H

#include <stddef.h>

int readline_rio(int fd, char* buffer, size_t max_len);
void write_rio(int fd, char* buffer, size_t total_len);

#endif