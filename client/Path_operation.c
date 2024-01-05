//
// Created by kyrie on 2023/12/30.
//

#include "Path_operation.h"
#include "protocol_client.h"
#include "Login.h"
#include "Communicate.h"
#include "TLS_com.h"
#include "../IO.h"
#include "../Crypto/secure_communication.h"

int LIST(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    setbuf(stdout, NULL);
    if(ftp->AUTHMODE){
        if(PASV(ftp) != 0){
            printf("LIST connect to server failed\n");
            return -1;
        }
    }else {
        if(PASV(ftp) != 0){
            printf("LIST connect to server failed\n");
            return -1;
        }
    }

    sendrequst(ftp);
    getresponse(ftp);//150
    showresponse(ftp);
    if(ftp->code == 550)goto LISTEND;
    if(ftp->code == 150){
        if(ftp->AUTHMODE){
            //Read2F(ftp->socket_pasv_data, stdout);
            SSL_Read2F(ftp->socket_pasv_data, stdout, additional, key, iv,CLIENT_SIDE);
        }else {
            Read2F(ftp->socket_pasv_data, stdout);
        }
    }

    LISTEND:
    getresponse(ftp);
    showresponse(ftp);
    close(ftp->socket_pasv_data);
    ftp->socket_pasv_data = 0;
    return 0;
}

int CWD(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);
    if(ftp->code == 250)return 0;
    return -1;
}

int PWD(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);
    if(ftp->code == 257)return 0;
    return -1;
}

