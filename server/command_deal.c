//
// Created by kyrie on 2023/12/30.
//

#include <ctype.h>
#include "command_deal.h"
#include "login.h"
#include "path_operation.h"
#include "tls_com.h"
#include "up_download.h"
#include "file_operation.h"



int parse_cmd(FTP *ftp){
    const char *buf = ftp->request;

    int n = 1024;
    int i, j;
    for (i = 0; i < min(10, n) && !isspace(buf[i]); ++i){
        ftp->cmd[i] = buf[i];
    }
    if(i < 10)ftp->cmd[i] = '\0';
    while(i < min(10, n) && buf[i] == ' ')++i;
    n -= i;
    for(j = 0; j < min(256, n) && buf[i] != '\r';++i, ++j ){
        ftp->arg[j] = buf[i];
    }
    if(j < 256)ftp->arg[j] = '\0';
    printf("cmd: %s\narg: %s\n", ftp->cmd, ftp->arg);

    if(!strncmp(ftp->cmd, "USER", sizeof("USER"))){
        ftp->dorequest = USER;
    }else if(!strncmp(ftp->cmd, "PASS", sizeof("PASS"))){
        ftp->dorequest = PASS;
    }else if(!strncmp(ftp->cmd, "BYE", sizeof("BYE"))){
        ftp->dorequest = BYE;
    }else if(!strncmp(ftp->cmd, "LIST", sizeof("LIST"))){
        ftp->dorequest = LIST;
    }else if(!strncmp(ftp->cmd, "CWD", sizeof("CWD"))){
        ftp->dorequest = CWD;
    }else if(!strncmp(ftp->cmd, "PWD", sizeof("PWD"))){
        ftp->dorequest = PWD;
    }else if(!strncmp(ftp->cmd, "PASV", sizeof("PASV"))){
        ftp->dorequest = PASV;
    }else if(!strncmp(ftp->cmd, "AUTH", sizeof("AUTH"))){
        ftp->dorequest = AUTH;
    }else if(!strncmp(ftp->cmd, "CCC", sizeof("CCC"))){
        ftp->dorequest = CCC;
    }else if(!strncmp(ftp->cmd, "RETR", sizeof("RETR"))){
        ftp->dorequest = RETR;
    }else if(!strncmp(ftp->cmd, "STOR", sizeof("STOR"))){
        ftp->dorequest = STOR;
    }
    else if(!strncmp(ftp->cmd, "MKDIR", sizeof("MKDIR"))){
        ftp->dorequest = MKDIR;
    }
    else if(!strncmp(ftp->cmd, "MKFILE", sizeof("MKFILE"))){
        ftp->dorequest = MKFILE;
    }
    else if(!strncmp(ftp->cmd, "RENAME", sizeof("RENAME"))){
        ftp->dorequest = RENAME;
    }
    else if(!strncmp(ftp->cmd, "RMFILE", sizeof("RMFILE"))){
        ftp->dorequest = RMFILE;
    }
    else{
        ftp->dorequest = NULL;
        return -1;
    }
    return 0;
}