//
// Created by kyrie on 2023/12/30.
//

#include "../network.h"
#include "Communicate.h"
#include "Command_get.h"

#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>

#include "init_client.h"
#include "../Crypto/msg.h"
#include "../Crypto/secure_communication.h"
// params.h
#define BUF_SIZE            256
#define MAX_FILE_SIZE       1000
#define MAX_CLNT            256


// command
#define SUCCESS             1
#define FAIL                (-1)
pthread_mutex_t mutx;





int init(FTP *ftp, const char *address, const uint16_t port){
    setbuf(stdout, NULL);
    int cnt_i;
    ftp->socket_cmd = 0;
    ftp->socket_pasv_data = 0;
    ftp->ctx = NULL;
    ftp->ssl = NULL;
    ftp->AUTHMODE = 0;
    ftp->ccc = 0;
    ftp->f = NULL;
    ftp->ftp_exit = ftp_exit;
    ftp->loop = loop;
    ftp->dorequest = NULL;




    ftp_init(ftp, address, port);
    //ftp->loop(ftp);
    return 0;
}
int ftp_init(FTP *ftp, const char *address, const uint16_t port){
    ftp->socket_cmd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    if (0 == inet_aton(address, &serveraddr.sin_addr)){
        printf("address: %s is invalid\n", address);
    }

    if(0 == connect(ftp->socket_cmd, (SA *)&serveraddr, sizeof(serveraddr))){
        printf("connect to %s successed!\n", Sock_ntop((SA *)&serveraddr));
    }


    return ftp->socket_cmd;
}

void ftp_exit(FTP *ftp){
    if(ftp->socket_cmd > 0){
        close(ftp->socket_cmd);
        ftp->socket_cmd = 0;
    }
    if(ftp->socket_pasv_data > 0){
        close(ftp->socket_pasv_data);
        ftp->socket_pasv_data = 0;
    }
    if(ftp->f != NULL)fclose(ftp->f);
    exit(0);
}

void loop(FTP *ftp){

    getresponse(ftp);
    showresponse(ftp);
    if(ftp->code != 200 && ftp->code != 220){
        ftp->ftp_exit(ftp);
    }

    while(1){
        printf("\nFTPS> ");
        char cmd[11+PATH_MAX];
        char *p = cmd;
        size_t n = 11+PATH_MAX;
        getline(&p, &n, stdin);
        if(-1 == getCmd(ftp, p, n)){
            continue;
        }
        ftp->dorequest(ftp);
    }
}
