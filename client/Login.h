//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_LOGIN_H
#define FTPS_LOGIN_H

#include "protocol_client.h"

//登录
int checklogin(FTP *);
int USER(FTP *);
int PASS(FTP *);
int BYE(FTP *);


#endif //FTPS_LOGIN_H
