//
// Created by kyrie on 2023/12/30.
//

#include "Login.h"
#include "protocol_client.h"
#include "Communicate.h"
#include "init_client.h"

int checklogin(FTP *ftp){
    if(ftp->loged == 0x11)return 0;//loged in
    if(ftp->loged == 0)return -1;//need user&passwd
    else if(ftp->loged == 0x10)return -2;//need passwd
    return -3;
}


int USER(FTP *ftp){
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code == 230 || ftp->code == 331){
        ftp->loged = 0x10;
        return 0;
    }
    return -1;
}

int PASS(FTP *ftp){
    if(checklogin(ftp) != -2){
        printf("USER name first please!\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);


    if(ftp->code == 230){
        ftp->loged = 0x11;
        return 0;
    }
    return -1;
}

int BYE(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);
    ftp_exit(ftp);
    return 0;
}
int SIGN(FTP *ftp){
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code == 666){

        return 0;
    }
    return -1;
}