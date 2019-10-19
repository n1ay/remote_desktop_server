#include "socketserver.h"

void *socket_server_start(void* socksrv_attr) {
    int s, sfd;
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, ((SocketServerAttr*) socksrv_attr)->port, &hints, &res);
    if (s != 0) {
        gai_strerror(s);
        return (void*) 1;
    }

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }

        close(sfd);
    }

    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        return (void*) 1;
    }
    freeaddrinfo(res);

    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char* initbuf = (char*) malloc(BUF_SIZE);
    memset(initbuf, 0, BUF_SIZE);
    while(1) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, initbuf, BUF_SIZE, 0, (struct sockaddr*) &peer_addr, &peer_addr_len);
        if (nread > 0) {
            break;
        }
    }

    free(initbuf);
    char host[NI_MAXHOST], service[NI_MAXSERV];
    s = getnameinfo((struct sockaddr*) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
    while (1) {
        sendto(sfd, *((SocketServerAttr*) socksrv_attr)->buf, *((SocketServerAttr*) socksrv_attr)->bufsize, 0, (struct sockaddr*) &peer_addr, peer_addr_len);
        sleep(1);
    }
}

void init_socksrv_attr (SocketServerAttr* socksrv_attr, char* port, unsigned char** buf, unsigned long* bufsize) {
    socksrv_attr->port = port;
    socksrv_attr->buf = buf;
    socksrv_attr->bufsize = bufsize;
}
