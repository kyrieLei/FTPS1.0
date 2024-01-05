#ifndef PROTOCOL_H
#define PROTOCOL_H




#include "../tls.h"
#include "../Crypto/msg.h"
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>


typedef struct __ftp{
    int socket_cmd;//命令通道
    int socket_pasv_data;//数据通道

    SSL_CTX *ctx;
    SSL *ssl;
    int AUTHMODE;
    int ccc;
    uint8_t loged;

    char response[2048];
    char request[1024];
    u_int16_t code;
    FILE *f;
    char filename[PATH_MAX];
    int (*dorequest)(struct __ftp *);
    void (*loop)(struct __ftp *);
    void (*ftp_exit)(struct __ftp *);
} FTP;



















#endif // PROTOCOL_H
