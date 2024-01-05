#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "../tls.h"
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#define min(a, b) ((a) < (b) ? (a) : (b))


typedef struct __ftp{
    int socket_listen;//父节点
    int socket_client;//命令端
    int socket_pasv_listen;//被动监听
    int socket_pasv_conn;//数据端被动连接

    SSL_CTX *ctx;//TLS 上下文
    SSL *ssl;//TLS 外壳
    int AUTHMODE;// 1表示TLS开
    int ccc; //1表示明文传输
    int (*ftp_init)(struct __ftp *);
    char response[2048];
    char request[1024];
    char cmd[10];
    char arg[PATH_MAX];
    char cur_dir[PATH_MAX];
    char ftp_path[PATH_MAX];
    ssize_t (*sendresponse)(struct __ftp *);
    ssize_t (*getrequst)(struct __ftp *);
    int (*dorequest)(struct __ftp *);
    unsigned char user_passwd;//高四位是user,低四位是passwd
    int (*parse_cmd)(struct __ftp *);
    void (*loop)(struct __ftp *);
    int (*ftp_handler)(struct __ftp *);
    void (*ftp_exit)(struct __ftp *);
} FTP;

static const char *USER_PASS[2][2]={{"l","l"}, {"z","z"}};
static uint16_t CMD_PORT = 9876;
static int QueueMax = 1024;
















#endif // PROTOCOL_H
