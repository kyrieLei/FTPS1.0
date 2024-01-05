//
// Created by kyrie on 2023/12/30.
//

#include <sys/stat.h>
#include "file_operation.h"

int MKDIR(FTP *ftp){
    if(strlen(ftp->arg) < 1){
        strcpy(ftp->response,"550 Missing directory name.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    if(mkdir(ftp->arg, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
        strcpy(ftp->response,"550 Directory make Failed!\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    strcpy(ftp->response,"335 Directory make successfully!\r\n");
    ftp->sendresponse(ftp);
    return 0;

}
int MKFILE(FTP *ftp){
    FILE* file = fopen(ftp->arg, "w"); // 以写入模式打开文件
    if (file == NULL) {
        strcpy(ftp->response,"550 Can't make a file :");
        strncat(ftp->response, ftp->arg, strlen(ftp->arg));
        ftp->sendresponse(ftp);
        return -1;
    }
    fprintf(file, "Here is Leijingdi\n");
    fclose(file);
    strcpy(ftp->response,"220 File make successfully!\r\n");
    ftp->sendresponse(ftp);


    return 0;
}
int RENAME(FTP *ftp){

    char *arg_1=strtok(ftp->arg," ");
    char *arg_2=strtok(NULL," ");

    if (strlen(arg_1)<1){
        strcpy(ftp->response,"550 Missing Original file name.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    if (strlen(arg_2)<1){
        strcpy(ftp->response,"550 Missing Target file name.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    if (rename(arg_1, arg_2) == -1){
        strcpy(ftp->response,"550 Rename Failed!\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    strcpy(ftp->response,"220 Rename successfully!\r\n");
    ftp->sendresponse(ftp);
    return 0;
}
int RMFILE(FTP *ftp){
    if (remove(ftp->arg) == -1) {
        strcpy(ftp->response,"550 Delete Failed!\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    strcpy(ftp->response,"220 Delete successfully!\r\n");
    ftp->sendresponse(ftp);
    return 0;
}
