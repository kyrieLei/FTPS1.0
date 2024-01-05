//
// Created by kyrie on 2023/12/30.
//

#include <sys/stat.h>
#include "File_operation.h"
#include "protocol_client.h"
#include "Login.h"
#include "Communicate.h"


int MKDIR(FTP *ftp){
    if(checklogin(ftp)!=0){
        printf("not login...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code==335) return 0;
    return -1;

}

int MKFILE(FTP *ftp){
    if(checklogin(ftp)!=0){
        printf("not login...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code==220) return 0;
    return -1;
}
int RENAME(FTP *ftp){
    if(checklogin(ftp)!=0){
        printf("not login...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code==220) return 0;
    return -1;

}
//删除文件或文件夹
int RMFILE(FTP *ftp){
    if(checklogin(ftp)!=0){
        printf("not login...\n");
        return -1;
    }
    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code==220) return 0;
    return -1;
}
