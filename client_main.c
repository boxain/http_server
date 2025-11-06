#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib/config.h"
#include "lib/open_client.h"

int g_runtime_debug = 0;

int main(int argc, char* argv[])
{
    if(argc > 1 && strcmp(argv[1], "-d") == 0){
        g_runtime_debug = 1;
        printf("[SYSTEM]-Runtime debug log is ENABLED.\r\n");
    }

    pthread_t threads[CONNECTION_COUNT];
    int res;
    for(int i = 0 ; i < CONNECTION_COUNT ; i++){
        int* thread_identify = malloc(sizeof(int));
        *thread_identify = (i+1);
        res = pthread_create(&(threads[i]), NULL, open_clientfd, (void* )thread_identify);
        if(res != 0){
#if DEBUG_LOG == 1
            printf("[ERROR]-failed to create thread:%d\r\n", *thread_identify);
#endif
        }
    }

    for(int i = 0 ; i < CONNECTION_COUNT ; i++){
        if (pthread_join(threads[i], NULL) != 0) {
#if DEBUG_LOG == 1
            printf("[ERROR]-failed to join thread: %d\r\n", i+1);
#endif
        }
    }
    return 0;
}