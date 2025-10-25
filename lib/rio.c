#include <stdio.h>
#include <unistd.h>


int readline_rio(int fd, char* buffer, size_t max_len)
{
    int result;
    char c;
    size_t read_bytes = 0;

    for(read_bytes = 0 ; read_bytes < max_len ; read_bytes++){
        result = read(fd, &c, 1);
        if(result == 1){
            *buffer = c;
            buffer++;
            
            if(c == '\n'){
                read_bytes++;
                break;
            }

        }else if(result == 0){
            if(read_bytes > 0){
                break;
            }else{
                return 0;
            }
        }else{
            return -1;
        }
    
    }

    *buffer = '\0';
    return read_bytes-1;
}

void write_rio(int fd, char* buffer, size_t total_len)
{
    size_t remain_bytes = total_len;

    while(remain_bytes > 0){
        ssize_t written_bytes = write(fd, buffer, remain_bytes);
        if(written_bytes == -1){
            printf("[ERROR]-failed to send http response to client: %d", fd);
            break;
        }else{
            buffer += written_bytes;
            remain_bytes -= written_bytes;
        }
    }
}