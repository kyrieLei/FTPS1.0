//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_PATH_OPERATION_H
#define FTPS_PATH_OPERATION_H


#include "protocol_client.h"

int LIST(FTP *);
int CWD(FTP *);
int PWD(FTP *);


#endif //FTPS_PATH_OPERATION_H
