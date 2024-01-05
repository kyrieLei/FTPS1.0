//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_TLS_COM_H
#define FTPS_TLS_COM_H


#include "protocol_server.h"

int PASV(FTP *);
int AUTH(FTP *);
int CCC(FTP *);


#endif //FTPS_TLS_COM_H
