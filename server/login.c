//
// Created by kyrie on 2023/12/30.
//

#include "login.h"
#include "communication.h"
#include "init_server.h"



int USER(FTP *ftp){
    for (int i = 0; i < 2; ++i){
        if(!strcmp(USER_PASS[i][0], ftp->arg)){
            ftp->user_passwd = 0;
            ftp->user_passwd |= ((i+1)<<4);
            strcpy(ftp->response, "331 Please sepcify the password.\r\n");
            ftp->sendresponse(ftp);
            return 0;
        }
    }
    strcpy(ftp->response, "550 User is not found.\r\n");
    ftp->sendresponse(ftp);
    return -1;
}

int PASS(FTP *ftp){
    if((ftp->user_passwd & 0x0F) != 0){
        strcpy(ftp->response, "530 alread loged. Try USER\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    unsigned char index = ((ftp->user_passwd & 0xF0) >> 4)-1;

    if(!strcmp(USER_PASS[index][1], ftp->arg)){
        ftp->user_passwd |= (index+1);
        strcpy(ftp->response, "230 login success\r\n");
    }
    ftp->sendresponse(ftp);
    return 0;
}

int BYE(FTP *ftp){
    strcpy(ftp->response, "221 Goodbye.\r\n");
    sendresponse(ftp);
    ftp_exit(NULL);
    return 0;
}
int SIGN(FTP *ftp){

}