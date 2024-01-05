//
// Created by kyrie on 2023/12/30.
//

#include <sys/socket.h>
#include "tls_com.h"
#include "../network.h"
#include "../IO.h"
#include "communication.h"
#include <unistd.h>
#include <sys/wait.h>



int PASV(FTP *ftp){
    struct sockaddr_in pasvaddr;
    socklen_t	pasvlen;
    uint16_t port;
    uint32_t ip;
    if(ftp->socket_pasv_listen > 0){
        close(ftp->socket_pasv_listen);
        ftp->socket_pasv_listen = 0;
    }
    ftp->socket_pasv_listen = Socket(AF_INET, SOCK_STREAM, 0);
    pasvlen = sizeof(pasvaddr);
    getsockname(ftp->socket_client, (struct sockaddr*)&pasvaddr, &pasvlen);
    pasvaddr.sin_port = htons(0);//let system choose
    Bind(ftp->socket_pasv_listen, (struct sockaddr*)&pasvaddr, sizeof(pasvaddr));

    Listen(ftp->socket_pasv_listen, QueueMax);

    pasvlen = sizeof(pasvaddr);
    getsockname(ftp->socket_pasv_listen, (SA*)&pasvaddr, &pasvlen);
    ip = (uint32_t)(pasvaddr.sin_addr.s_addr);
    port = (uint16_t)(pasvaddr.sin_port);
    snprintf(ftp->response,1024, "%d Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n", 227,
          (ip >> 24) & 0xff,(ip >> 16) & 0xff, (ip >> 8) & 0xff,
            ip & 0xff, (port >> 8) & 0xff, port & 0xff);
    ftp->sendresponse(ftp);
    return 0;
}

int AUTH(FTP *ftp){

    pasvconn(ftp);

    strcpy(ftp->response, "123 AUTHING...Please try connect\r\n");
    sendresponse(ftp);

    ftp->AUTHMODE = 1;

    strcpy(ftp->response, "334 AUTH successed. Data channel will be take over by TLS.\r\n");
    ftp->sendresponse(ftp);
    return 0;


}

int CCC(FTP *ftp){
    if(!ftp->AUTHMODE){
        strcpy(ftp->response, "550 Not authed yet.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    if(ftp->ccc){
        ftp->AUTHMODE=0;
        strcpy(ftp->response, "200 Already cancled.\r\n");
        ftp->sendresponse(ftp);
        return 0;
    }
    if(!ftp->ccc&&ftp->AUTHMODE){
        ftp->AUTHMODE=0;
        strcpy(ftp->response, "200 Command channel protection cancled.\r\n");
        ftp->sendresponse(ftp);
        ftp->ccc = 1;
        return 0;
    }
    return -1;
}