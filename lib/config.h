#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_LOG     1      // Print error level log
#define CONN_TEST     1      // Let each http request delay 30 seconds.
#define MAXLINE       128    // Client host and service max length
#define USE_THREADING 0      // Use thread to process new request
#define CONNECTION_COUNT 10  // How many test client to create

extern int g_runtime_debug;  // Print runtime info level log

#endif