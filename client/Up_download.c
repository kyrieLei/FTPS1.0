//
// Created by kyrie on 2023/12/30.
//

#include <sys/stat.h>
#include "Up_download.h"
#include "protocol_client.h"
#include "Login.h"
#include "TLS_com.h"
#include "Communicate.h"
#include "../IO.h"
#include "../Crypto/secure_communication.h"

int RETR(FTP *ftp){

    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }

    if(ftp->AUTHMODE){
        if(PASV(ftp) != 0){
            printf("RETR connect to server failed\n");
            return -1;
        }
    }else {
        if(PASV(ftp) != 0){
            printf("RETR connect to server failed\n");
            return -1;
        }
    }


    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);
    if(ftp->code != 150){

        fclose(ftp->f);
        ftp->f = NULL;
        remove(ftp->filename);

        close(ftp->socket_pasv_data);
        ftp->socket_pasv_data = 0;

        return -1;
    }
    if(ftp->code == 150){

        int ret;
        if(ftp->AUTHMODE){
            SSL_Read2F(ftp->socket_pasv_data, ftp->f,additional, key, iv, CLIENT_SIDE);
        }else {
            Read2F(ftp->socket_pasv_data, ftp->f);
        }

        getresponse(ftp);//226
        showresponse(ftp);
    }

    fclose(ftp->f);
    ftp->f = NULL;

    close(ftp->socket_pasv_data);
    ftp->socket_pasv_data = 0;

    return 0;
}

int STOR(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    if(ftp->AUTHMODE){
        if(PASV(ftp) != 0){
            printf("STOR connect to server failed\n");
            return -1;
        }
    }else {
        if(PASV(ftp) != 0){
            printf("STOR connect to server failed\n");
            return -1;
        }
    }

    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code != 150){
        fclose(ftp->f);
        ftp->f = NULL;
        if(!ftp->AUTHMODE){
            close(ftp->socket_pasv_data);
            ftp->socket_pasv_data = 0;
        }
        return -1;
    }
    struct stat st;


            char buff[BUFSIZ];
            int n;
            do{
                n = fread(buff, sizeof(char), BUFSIZ, ftp->f);
                if(ftp->AUTHMODE){
                    if(-1 == SSL_send(ftp->socket_pasv_data, buff, additional, key, iv)){
                        printf("STOR write failed\n");
                        break;
                    }

                }else {
                    if(-1 == Writen(ftp->socket_pasv_data, buff, n)){
                        printf("STOR write failed\n");
                        break;
                    }
                }
            }while(!feof(ftp->f));

            close(ftp->socket_pasv_data);
            ftp->socket_pasv_data = 0;

            getresponse(ftp);
            showresponse(ftp);



    return 0;
}
