//
// Created by kyrie on 2023/12/30.
//

#include <sys/stat.h>
#include <sys/file.h>
#include "up_download.h"
#include "communication.h"
#include "../IO.h"
#include "../Crypto/secure_communication.h"

int RETR(FTP *ftp){
    char buff[BUFSIZ];
    size_t n;

//    printf("%s\n",ftp->arg);
//    if(!ftp->AUTHMODE)
    pasvconn(ftp);
    //printf("%s\n",ftp->arg);


        FILE *f = fopen(ftp->arg, "r");
        printf("%s\n",ftp->arg);
        if(NULL == f){
            printf("RETR: %s don't exsit\n", ftp->arg);
            goto RETRERR;
        }
        strcpy(ftp->response, "150 Here comes the file.\r\n");
        sendresponse(ftp);
        do{
            n = fread(buff, sizeof(char), BUFSIZ, f);
            if(ferror(f)){
                perror("RETR fread failed:");
                break;
            }
            if(ftp->AUTHMODE){
                SSL_send(ftp->socket_pasv_conn, buff, additional, key, iv);
            }else{
                Writen(ftp->socket_pasv_conn, buff, n);
            }
        }while(!feof(f));
        fclose(f);
        strcpy(ftp->response, "226 File send OK.\r\n");
        closesocket(ftp);
        ftp->sendresponse(ftp);
        return 0;

    RETRERR:
    closesocket(ftp);
    snprintf(ftp->response,1024,
            "550 File \"%s\" specified not exists.\r\n", ftp->arg);
    ftp->sendresponse(ftp);
    return -1;


}
int STOR(FTP *ftp){

    if(ftp->AUTHMODE){
        pasvconn(ftp);
        if(ftp->socket_pasv_conn <= 0){
            printf("STOR: TLS not established\n");
            closesocket(ftp);
            sprintf(ftp->response,
                    "550 STRO failed on AUTH.\r\n");
            ftp->sendresponse(ftp);
            return -1;
        }
    }else{
        pasvconn(ftp);
    }


        char*buff;
        FILE *f = fopen(ftp->arg, "w");
        if(NULL == f){
            printf("STOR: %s can't store\n", ftp->arg);
            closesocket(ftp);
            snprintf(ftp->response,1024,
                    "550 File \"%s\" file stored Failed.\r\n", ftp->arg);
            ftp->sendresponse(ftp);
            return -1;
        }


        strcpy(ftp->response, "150 Ready to accept file.\r\n");
        sendresponse(ftp);


        long ret=0;

        if(ftp->AUTHMODE){
            //ret = Read2F(ftp->socket_pasv_conn, f);
            ret = SSL_Read2F(ftp->socket_pasv_conn, f, additional, key, iv, SERVER_SIDE);
        }else {
            ret = Read2F(ftp->socket_pasv_conn, f);
        }

        if(ret == 0){
            printf("STOR file success\n");
        }else if(ret == -1){
            printf("STOR: socket error\n");
        }else{
            printf("STOR file failed\n");
        }

        if(ferror(f)){
            fclose(f);
            closesocket(ftp);
            snprintf(ftp->response,1024,
                    "550 File \"%s\" file stored Failed.\r\n", ftp->arg);
            ftp->sendresponse(ftp);
            return -1;
        }

        fclose(f);
        strcpy(ftp->response, "226 File stored OK.\r\n");
        closesocket(ftp);
        ftp->sendresponse(ftp);
        return 0;


}

