//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_INIT_SERVER_H
#define FTPS_INIT_SERVER_H


#include "protocol_client.h"

int init(FTP *, const char *address, uint16_t port);
int ftp_init(FTP *, const char *address, uint16_t port);
void loop(FTP *);
void ftp_exit(FTP *);
int sign(int);

#endif //FTPS_INIT_SERVER_H
