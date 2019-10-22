#include "socketserver.h"

void* socket_server_start(void* socksrv_attr) {
    int s, sfd, new_fd, max_fd, thr_idx;
    struct addrinfo hints, *res, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    fd_set master, read_fds;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    ClientThread cts[MAX_CLIENTS];
    ClientThreadAttr ctas[MAX_CLIENTS];
    char threads[MAX_CLIENTS];
    memset(threads, 0, MAX_CLIENTS * sizeof(char));
    
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char* readbuf = (char*) malloc(BUF_SIZE);
    memset(readbuf, 0, BUF_SIZE);

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
                        thr_idx = find_free_thread(threads);
                        if (thr_idx == -1) {
                            // all threads are used; drop the connection
                            close(new_fd);
                        } else {
                            // there are some free threads in thread pool, start new thread then
                            printf("Starting thread [%d]\n", thr_idx);
                            threads[thr_idx] = 1;
                            cts[thr_idx].fd = new_fd;
                            init_ct_attr(&ctas[thr_idx], new_fd, socksrv_attr);
                            pthread_create(&cts[thr_idx].thread, NULL, send_worker, (void*) &ctas[thr_idx]);
                        }
                    }
                } else {
                    // data from already connected client
                    nread = recv(i, readbuf, BUF_SIZE, 0);
                    if (nread <= 0) {
                        if (nread == -1) {
                            perror("recv");
                        } else {
                            printf("Socket %d hung up\n", i);
                        }
                        close(i);
                        FD_CLR(i, &master);
                        thr_idx = find_thread_by_fd(i, threads, cts);
                        if (thr_idx != -1) {
                            threads[thr_idx] = 0;
                            pthread_cancel(cts[thr_idx].thread);
                            printf("Cancelling thread [%d]\n", thr_idx);
                        }
                    }
                }
            }
        }
    }
}

void init_socksrv_attr (SocketServerAttr* socksrv_attr, char* port, unsigned char** buf, unsigned long* bufsize) {
    socksrv_attr->port = port;
    socksrv_attr->buf = buf;
    socksrv_attr->bufsize = bufsize;
}

void init_client_thread(ClientThread* client_thread, int fd, pthread_t thread) {
    client_thread->fd = fd;
    client_thread->thread = thread;
}

int find_thread_by_fd(int fd, char threads[], ClientThread cts[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (threads[i] && cts[i].fd == fd) {
            return i;
        }
    }

    return -1;
}

void init_ct_attr(ClientThreadAttr* ct_attr, int fd, SocketServerAttr* socksrv_attr) {
    ct_attr->fd = fd;
    ct_attr->buf = socksrv_attr->buf;
    ct_attr->bufsize = socksrv_attr->bufsize;
}

void* send_worker(void* ct_attr) {
    while(1) {
        send(((ClientThreadAttr*) ct_attr)->fd, *((ClientThreadAttr*) ct_attr)->buf, *((ClientThreadAttr*) ct_attr)->bufsize, MSG_NOSIGNAL);
    }
}

int find_free_thread(char threads[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!threads[i]) {
            return i;
        }
    }

    return -1;
}
