//
// Created by kyrie on 2023/12/30.
//

#ifndef FTPS_COMMUNICATION_H
#define FTPS_COMMUNICATION_H


#include "protocol_server.h"

long sendresponse(FTP*);

long sendstring(FTP *, const char* );
long getrequest(FTP *);
int closesocket(FTP *);
int pasvconn(FTP *);


#endif //FTPS_COMMUNICATION_H
