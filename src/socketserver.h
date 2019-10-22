#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define BACKLOG 10

#define BUF_SIZE 256

#define MAX_CLIENTS 10

typedef struct {
    int fd;
    pthread_t thread;
} ClientThread;

typedef struct {
    int fd;
    unsigned char** buf;
    unsigned long* bufsize;
} ClientThreadAttr;

typedef struct {
    char* port;
    unsigned char** buf;
    unsigned long* bufsize;
} SocketServerAttr;

void init_socksrv_attr(SocketServerAttr* socksrv_attr, char* port, unsigned char** buf, unsigned long* bufsize);

void* socket_server_start(void* socket_server_attr);

void init_client_thread(ClientThread* client_thread, int fd, pthread_t thread);

void init_ct_attr(ClientThreadAttr* ct_attr, int fd, SocketServerAttr* socksrv_attr);

void* send_worker(void* ct_attr);

int find_thread_by_fd(int fd, char threads[], ClientThread cts[]);

int find_free_thread(char threads[]);
