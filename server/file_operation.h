//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_FILE_OPERATION_H
#define FTPS_FILE_OPERATION_H


#include "protocol_server.h"

int MKDIR(FTP *);
int MKFILE(FTP *);
int RENAME(FTP *);
int RMFILE(FTP *);
int EDITOR(FTP *);

#endif //FTPS_FILE_OPERATION_H
