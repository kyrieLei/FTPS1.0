//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_LOGIN_H
#define FTPS_LOGIN_H


#include "protocol_server.h"

int USER(FTP *);
int PASS(FTP *);
int BYE(FTP *);
int SIGN(FTP *);

#endif //FTPS_LOGIN_H
