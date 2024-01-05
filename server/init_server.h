//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_INIT_SERVER_H
#define FTPS_INIT_SERVER_H


#define MAX_CLNT            256



#include "protocol_server.h"



int init(FTP *);
int ftp_init(FTP *);
void loop(FTP *);
int ftp_handler(FTP *);
void ftp_exit(FTP *);



#endif //FTPS_INIT_SERVER_H
