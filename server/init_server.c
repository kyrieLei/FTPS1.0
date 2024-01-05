//
// Created by kyrie on 2023/12/30.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "init_server.h"
#include "../network.h"
#include "../IO.h"
#include "communication.h"
#include "command_deal.h"
#include "../Crypto/msg.h"
#include "../Crypto/secure_communication.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#define BUF_SIZE            256
#define MAX_FILE_SIZE       1000
#define MAX_CLNT            256


// command
#define SUCCESS             1
#define FAIL                (-1)



int clnt_socks[MAX_CLNT];
int clnt_cnt;




int ftp_init(FTP *ftp){

    ftp->socket_listen = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;

    setsockopt(ftp->socket_listen, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    struct sockaddr_in serveraddr;

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(CMD_PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(ftp->socket_listen, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    listen(ftp->socket_listen, QueueMax);
    printf("serveraddr: %s\n", Sock_ntop((struct sockaddr*)&serveraddr));


    Getcwd(ftp->cur_dir, sizeof(ftp->cur_dir));
    return ftp->socket_listen;
}
int init(FTP *ftp){
    ftp->socket_client = 0;
    ftp->socket_listen = 0;
    ftp->socket_pasv_listen = 0;
    ftp->socket_pasv_conn = 0;
    //ftp->socket_tls = NULL;
    ftp->AUTHMODE = 0;//TLS is not enabled
    ftp->ccc = 0;
    ftp->user_passwd = 0;
    ftp->ftp_init = ftp_init;
    ftp->ftp_handler = ftp_handler;
    ftp->ftp_exit = ftp_exit;
    ftp->parse_cmd = parse_cmd;
    ftp->loop = loop;
    ftp->sendresponse = sendresponse;
    ftp->getrequst = getrequest;
    ftp->dorequest = NULL;
    ftp->ftp_init(ftp);
}



void ftp_exit(FTP *ftp){
    if(ftp->socket_client > 0)
        close(ftp->socket_client);
    if(ftp->socket_listen > 0)
        close(ftp->socket_listen);
    if(ftp->socket_pasv_conn > 0)
        close(ftp->socket_pasv_conn);
    if(ftp->socket_pasv_listen > 0)
        close(ftp->socket_pasv_listen);
    exit(0);
}
void loop(FTP *ftp){



    BIO *bp_public = NULL;
    BIO *bp_private = NULL;
    BIO *rsa_pubkey = NULL;
    BIO *rsa_privkey = NULL;

    bp_public = BIO_new_file("util/public.pem", "r");
    if ( !PEM_read_bio_RSAPublicKey(bp_public, &rsa_pubkey, NULL, NULL) ) {
        close(ftp->socket_listen);
        return ;
    }

    bp_private = BIO_new_file("util/private.pem", "r");
    if ( !PEM_read_bio_RSAPrivateKey(bp_private, &rsa_privkey, NULL, NULL) ) {
        close(ftp->socket_listen);
        return ;
    }
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    while(1){


        clnt_addr_size = sizeof(clnt_addr);

        ftp->socket_client = Accept(ftp->socket_listen,  NULL, NULL);
        rsaes_setup_process(ftp->socket_client, key, rsa_pubkey, rsa_privkey, SERVER_SIDE);
        if (-1 == ftp->socket_client) continue;
        fflush(stdout);
        close(ftp->socket_listen);

        ftp->socket_listen = 0;
        chdir(ftp->cur_dir);

        ftp->ftp_handler(ftp);

        close(ftp->socket_client);
        ftp->socket_client = 0;


    }
}
int ftp_handler(FTP *ftp){


    strcpy(ftp->response, "200 welcome!\r\n");

    ftp->sendresponse(ftp);


    while(1){
        if(0 != ftp->getrequst(ftp)){
            ftp_exit(ftp);
        }

        if(0 != parse_cmd(ftp)){
            strcpy(ftp->response, "500 Unsupported cmd\r\n");
        }else{
            ftp->dorequest(ftp);
        }
    }

}