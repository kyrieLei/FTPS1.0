//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_COMMUNICATE_H
#define FTPS_COMMUNICATE_H


#include "protocol_client.h"
//通信
void showresponse(FTP *);
long getresponse(FTP *);
long sendrequst(FTP *);
long sendstring(FTP *ftp, const char* str);


#endif //FTPS_COMMUNICATE_H
