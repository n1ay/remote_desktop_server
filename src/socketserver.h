#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 10

#define BUF_SIZE 256

typedef struct {
    char* port;
    unsigned char** buf;
    unsigned long* bufsize;
} SocketServerAttr;

void init_socksrv_attr(SocketServerAttr* socksrv_attr, char* port, unsigned char** buf, unsigned long* bufsize);

void* socket_server_start(void* socket_server_attr);
