//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_TLS_COM_H
#define FTPS_TLS_COM_H


#include "protocol_client.h"

//ftp
//被动传输
int PASV(FTP *);
//TLS身份验证请求
int AUTH(FTP *);
//切换为明文传输
int CCC(FTP *);


#endif //FTPS_TLS_COM_H
