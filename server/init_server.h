//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_INIT_SERVER_H
#define FTPS_INIT_SERVER_H


#include "protocol_server.h"
typedef struct __PIDCHILD{
    pid_t *node;
    int size;
    int maxsize;
} PIDCHILD;


int init(FTP *);
int ftp_init(FTP *);
void loop(FTP *);
int ftp_handler(FTP *);
void ftp_exit(FTP *);



#endif //FTPS_INIT_SERVER_H
