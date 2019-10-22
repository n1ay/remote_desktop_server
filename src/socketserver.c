#include "socketserver.h"

void* socket_server_start(void* socksrv_attr) {
    int s, sfd, new_fd, max_fd;
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    fd_set master, read_fds;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

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

    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char* readbuf = (char*) malloc(BUF_SIZE);
    memset(readbuf, 0, BUF_SIZE);
    
    if (listen(sfd, BACKLOG)) {
        perror("listen");
    }

    FD_SET(sfd, &master);
    max_fd = sfd;
    
    while(1) {
        read_fds = master;
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sfd) {
                    // new connection                 
                    if ((new_fd = accept(sfd, (struct sockaddr*) &peer_addr, &peer_addr_len)) == -1) {
                        perror("accept");
                    } else {
                        FD_SET(new_fd, &master);
                        if (new_fd > max_fd) {
                            max_fd = new_fd;
                        }
                        printf("New connection from %s on socket %d\n", inet_ntoa(peer_addr.sin_addr), new_fd);
                    }
                } else {
                    // data from already connected client
                    nread = recv(i, readbuf, sizeof(readbuf), 0);
                    if (nread <= 0) {
                        if (nread == -1) {
                            perror("recv");
                        } else {
                            printf("Socket %d hung up\n", i);
                        }
                        close(i);
                        FD_CLR(i, &master);
                    } else {
                        send(i, readbuf, nread, 0);
                    }
                }
            }
        }
    }

    /*while(1) {
        send(new_fd, *((SocketServerAttr*) socksrv_attr)->buf, *((SocketServerAttr*) socksrv_attr)->bufsize, 0);
    }*/
}

void init_socksrv_attr (SocketServerAttr* socksrv_attr, char* port, unsigned char** buf, unsigned long* bufsize) {
    socksrv_attr->port = port;
    socksrv_attr->buf = buf;
    socksrv_attr->bufsize = bufsize;
}
