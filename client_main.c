#define CONNECTION_COUNT 10

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/open_client.h"


int main()
{
    pthread_t threads[CONNECTION_COUNT];
    int res;
    for(int i = 0 ; i < CONNECTION_COUNT ; i++){
        int* thread_identify = malloc(sizeof(int));
        *thread_identify = (i+1);
        res = pthread_create(&(threads[i]), NULL, open_clientfd, (void* )thread_identify);
        if(res != 0){
            printf("[ERROR]-failed to create thread:%d\r\n", *thread_identify);
        }
    }

    for(int i = 0 ; i < CONNECTION_COUNT ; i++){
        if (pthread_join(threads[i], NULL) != 0) {
            printf("[ERROR]-failed to join thread: %d\r\n", i+1);
        }
    }
    
    return 0;
}