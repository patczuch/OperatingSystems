#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define MESSAGE_MAX 512
//#define IP "127.0.0.1"
//#define PORT 3208

typedef enum
{
    STOP = 1,
    LIST = 2,
    INIT = 3,
    TO_ALL = 4,
    TO_ONE = 5,
    REFRESH = 6
}
message_types;

typedef struct 
{
    int mtype;
    char mtext[MESSAGE_MAX];
}
message;

#endif