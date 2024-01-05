//
// Created by kyrie on 2023/12/30.
//

#include "Communicate.h"
#include "../IO.h"
#include "../Crypto/secure_communication.h"

void showresponse(FTP *ftp){
    fflush(stdout);
    printf("%s\n", ftp->response);
}
long sendrequst(FTP *ftp){
    return sendstring(ftp, ftp->request);
}
long sendstring(FTP *ftp, const char* str){
    int bufsz = strlen(str);

    if(ftp->AUTHMODE && !ftp->ccc){
        return SSL_send(ftp->socket_cmd, str, additional, key, iv);
    }else{
        return Writen(ftp->socket_cmd, str, bufsz);
    }
}


long getresponse(FTP *ftp){
    ERR_clear_error();
    fd_set rs;
    int trueblocking = 1;
    int fd = ftp->socket_cmd;
    char *buff = ftp->response;
    int sz = 1024;
    int bytesread = 0;
    int err;
    int kt;
    char *buffer;

    int fileerror = 0;
    if(ftp->AUTHMODE && !ftp->ccc){

            SSL_receive(fd, buff,additional, key, iv, CLIENT_SIDE);
            char buf[4];
            strncpy(buf, buff, 3);
            buf[3] = '\0';
            ftp->code = (uint16_t)atoi(buf);

            return 0;
    }
    else {
        do{
            if(trueblocking){
                FD_ZERO(&rs);
                FD_SET(fd, &rs);
                select(fd+1, &rs, NULL, NULL, NULL);
            }
            err = read(fd, buff+bytesread, 1);
            if(err < 0){
                if(EINTR == errno){
                    trueblocking = 0;
                    continue;
                }else if(EWOULDBLOCK == errno){
                    trueblocking = 1;
                    continue;
                }else{

                    return -1;
                }
            }else if(err == 0){
                if(fileerror) return -2;
                return 0;
            }
            trueblocking = 0;

            if(*(buff+bytesread) == '\n'){
                *(buff+bytesread) = '\0';
                char buf[4];
                strncpy(buf, ftp->response, 3);
                buf[3] = '\0';
                ftp->code = (uint16_t)atoi(buf);
                return 0;
            }
            if(++bytesread > sz){
                printf("getrequest: bufsz is too small or bad request format...\n");
                return -1;
            }
        }while(1);
    }

}
