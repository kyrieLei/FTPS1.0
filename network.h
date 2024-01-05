#ifndef __NETWORK_H__
#define __NETWORK_H__
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

typedef struct sockaddr SA;
int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
int Listen(int socket, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

char *Sock_ntop(const struct sockaddr *sa);

#endif
