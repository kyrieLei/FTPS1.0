//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_UP_DOWNLOAD_H
#define FTPS_UP_DOWNLOAD_H


#include "protocol_client.h"

//下载和上传
int RETR(FTP *);
int STOR(FTP *);
int FPUT(FTP *);
int FGET(FTP *);

#endif //FTPS_UP_DOWNLOAD_H
