//
// Created by kyrie on 2023/12/30.
//


#include <ctype.h>
#include "Command_get.h"
#include "Login.h"
#include "TLS_com.h"
#include "Path_operation.h"
#include "File_operation.h"
#include "Up_download.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

int getCmd(FTP *ftp, const char *s, int n){
    char cmd[11];
    char arg[256];

    int i = 0, j = 0;
    while(i < min(10, n) && !isspace(s[i]) && s[i] != '\n'){
        cmd[i] = s[i];
        ++i;
    }
    if(i < 11) cmd[i] = '\0';
    while(i < n && isspace(s[i]) && s[i] != '\n')++i;
    n -= i;
    while(j < 255 && i < min(255, n) && s[i] != '\n'){
        arg[j] = s[i];
        ++i, ++j;
    }
    if(j < 256)arg[j] = '\0';

    if(!strncmp(cmd, "user", sizeof("user"))){
        strncpy(ftp->request, "USER ", sizeof("USER "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = USER;
    }else if(!strncmp(cmd, "pass", sizeof("pass"))){
        strncpy(ftp->request, "PASS ", sizeof("PASS "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = PASS;
    }else if(!strncmp(cmd, "quit", sizeof("quit"))){
        strncpy(ftp->request, "BYE ", sizeof("BYE "));
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = BYE;
    }else if(!strncmp(cmd, "auth", sizeof("auth"))){
        strncpy(ftp->request, "AUTH ", sizeof("AUTH "));
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = AUTH;
    }else if(!strncmp(cmd, "ccc", sizeof("ccc"))){
        strncpy(ftp->request, "CCC ", sizeof("CCC "));
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = CCC;
    }else if(!strncmp(cmd, "passive", sizeof("passive"))){
        strncpy(ftp->request, "PASV ", sizeof("PASV "));
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = PASV;
    }else if(!strncmp(cmd, "ls", sizeof("ls"))){
        strncpy(ftp->request, "LIST ", sizeof("LIST "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = LIST;
    }else if(!strncmp(cmd, "pwd", sizeof("pwd"))){
        strncpy(ftp->request, "PWD ", sizeof("PWD "));
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = PWD;
    }else if(!strncmp(cmd, "cd", sizeof("cd"))){
        strncpy(ftp->request, "CWD ", sizeof("CWD "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = CWD;
    }
    else if(!strncmp(cmd, "cpwd", sizeof("cpwd"))){
        system("pwd");
        ftp->dorequest = NULL;
        return -1;
    }else if(!strncmp(cmd, "ccd", sizeof("ccd"))){
        char *str1;
        strncpy(str1,arg,j);
        snprintf(cmd, sizeof(cmd),"cd %s", str1);
        system(cmd);
        ftp->dorequest = NULL;
        return -1;
    }
    else if(!strncmp(cmd, "cls", sizeof("cls"))){
        system("ls");
        ftp->dorequest = NULL;
        return -1;
    }
    else if(!strncmp(cmd, "mkdir", sizeof("mkdir"))){
        strncpy(ftp->request, "MKDIR ", sizeof("MKDIR "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = MKDIR;
    }else if(!strncmp(cmd, "rename", sizeof("rename "))){
        strncpy(ftp->request, "RENAME ", sizeof("RENAME "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = RENAME;
    }else if(!strncmp(cmd, "touch", sizeof("touch "))){
        strncpy(ftp->request, "MKFILE ", sizeof("MKFILE "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = MKFILE;
    }
    else if(!strncmp(cmd, "sign", sizeof("sign "))){
        strncpy(ftp->request, "SIGN ", sizeof("SIGN "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = SIGN;
    }
    else if(!strncmp(cmd, "rm", sizeof("rm"))){
        strncpy(ftp->request, "RMFILE ", sizeof("RMFILE "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = RMFILE;
    }
    else if(!strncmp(cmd, "get", sizeof("get"))){
        ftp->f = fopen(arg, "w");
        if(NULL == ftp->f){
            printf("can't create local file: %s\n", arg);
            return -1;
        }
        strncpy(ftp->filename, arg, strlen(arg)+1);

        strncpy(ftp->request, "RETR ", sizeof("RETR "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = RETR;
    }else if(!strncmp(cmd, "put", sizeof("put"))){
        FILE *f = fopen(arg, "r");
        if(NULL == f){
            printf("file: %s doesn't exsit\n", arg);
            return -1;
        }
        ftp->f = f;
        strncpy(ftp->filename, arg, strlen(arg)+1);

        strncpy(ftp->request, "STOR ", sizeof("STOR "));
        strncat(ftp->request, arg, j);
        strncat(ftp->request, "\r\n", sizeof("\r\n"));
        ftp->dorequest = STOR;
    }
    else {
        printf("Unknow command: %s\n", cmd);
        ftp->dorequest = NULL;
        return -1;
    }

    return 0;
}