//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_FILE_OPERATION_H
#define FTPS_FILE_OPERATION_H


#include "protocol_client.h"

//文件操作
int MKDIR(FTP *);
int MKFILE(FTP *);
int RENAME(FTP *);
int RMFILE(FTP *);


#endif //FTPS_FILE_OPERATION_H
