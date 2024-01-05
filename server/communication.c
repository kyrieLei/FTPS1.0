//
// Created by kyrie on 2023/12/30.
//

#include <sys/socket.h>
#include "communication.h"
#include "../IO.h"
#include "init_server.h"
#include "../network.h"
#include "../Crypto/secure_communication.h"
#include "../client/protocol_client.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

long sendresponse(FTP *ftp){
    return sendstring(ftp, ftp->response);
}
long sendstring(FTP *ftp, const char* str){
    int bufsz = strlen(str);
    if(ftp->AUTHMODE && !ftp->ccc){
        return SSL_send(ftp->socket_client, str, additional, key, iv);
    }else{
        return Writen(ftp->socket_client, str, bufsz);
    }
}
long getrequest(FTP *ftp){
    fd_set rs;
    int trueblocking = 1;
    int fd = ftp->socket_client;
    char *buff = ftp->request;
    int sz = 1024;
    int bytesread = 0;
    int err;

    if(ftp->AUTHMODE && !ftp->ccc){

            if(trueblocking){
                FD_ZERO(&rs);
                FD_SET(fd, &rs);
                select(fd+1, &rs, NULL, NULL, NULL);
            }
            ERR_clear_error();
            SSL_receive(fd, buff,additional, key, iv, SERVER_SIDE);


            return 0;

    }else {

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
                return -1;
            }
            trueblocking = 0;

            if(*(buff+bytesread) == '\n')return 0;
            if(++bytesread > sz){
                printf("getrequest: bufsz is too small or bad request format...\n");
                return -1;
            }
        }while(1);
    }
}
int closesocket(FTP *ftp){
    if(ftp->socket_pasv_conn > 0){
        close(ftp->socket_pasv_conn);
        ftp->socket_pasv_conn = 0;
    }
    if(ftp->socket_pasv_listen > 0){
        close(ftp->socket_pasv_listen);
        ftp->socket_pasv_listen = 0;
    }
    return 0;
}
int pasvconn(FTP *ftp){
    if(ftp->socket_pasv_conn > 0) return ftp->socket_pasv_conn;
    if(ftp->socket_pasv_listen < 0){
        printf("pasv listen socket closed!\n");
        ftp_exit(ftp);
        return -1;
    }
    ftp->socket_pasv_conn = Accept(ftp->socket_pasv_listen, NULL, NULL);
    if(ftp->socket_pasv_conn != -1){
        {
            struct sockaddr_in pasvaddr = {};
            socklen_t	pasvlen;
            getsockname(ftp->socket_pasv_conn, (struct sockaddr *)&pasvaddr, &pasvlen);
            printf("child passive data channel port: %d\n", ntohs(pasvaddr.sin_port));
        }

        return ftp->socket_pasv_conn;
    }
    printf("accept pasv connection failed\n");
    ftp_exit(ftp);
    return -1;
}