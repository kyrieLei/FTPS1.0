//
// Created by kyrie on 2023/12/30.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "TLS_com.h"
#include "protocol_client.h"
#include "Login.h"
#include "Communicate.h"
#include "../network.h"
#include "../IO.h"
#include <sys/wait.h>




int PASV(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }


    if(ftp->socket_pasv_data > 0)return 0;

    char *cmd = "PASV \r\n";

    sendstring(ftp, cmd);
    getresponse(ftp);
    showresponse(ftp);

    if(ftp->code != 227){
        printf("PASV failed: %s", ftp->response);
        return -1;
    }

    uint16_t buff[6];
    uint16_t port;
    uint32_t ip;
    ftp->socket_pasv_data = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;

    char *p = strchr(ftp->response, '(');
    if (p == NULL){
        printf("not ip & port info");
        return -1;
    }
    ++p;
    for(int i = 0; i < 6; ++p, ++i){
        buff[i] = atoi(p);
        p = strchr(p, ',');
        if(p == NULL && i < 5){
            printf("ip & port bad info");
            return -1;
        }
    }

    ip = (buff[0]<<24)|(buff[1]<<16)|(buff[2]<<8)|buff[3];
    port = (buff[4]<<8)|buff[5];
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = port;
    serveraddr.sin_addr.s_addr = ip;

    if(0 !=connect(ftp->socket_pasv_data, (struct sockaddr *)&serveraddr, sizeof(serveraddr))){
        perror("pasv connect failed:");
        return -1;
    }
//    int fdflag = fcntl(ftp->socket_pasv_data, F_GETFL, 0);
//    fcntl(ftp->socket_pasv_data, F_SETFL, fdflag | O_NONBLOCK);
    //fcntl(ftp->socket_pasv_data, F_SETFL, fdflag & ~O_NONBLOCK);
    return 0;
}

int AUTH(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }

    if(ftp->AUTHMODE && ftp->socket_pasv_data > 0){
        printf("AUTHED!\n");
        return 0;
    }
    if(PASV(ftp) != 0){
        printf("AUTH connect to server failed\n");
        return -1;
    }



    sendrequst(ftp);
    getresponse(ftp);
    showresponse(ftp);


    if(ftp->code != 123){
        printf("Reasons about server failed to initilizae TLS:\n");
        getresponse(ftp);
        showresponse(ftp);
        ftp->AUTHMODE = 0;
        return -1;
    }

    ftp->AUTHMODE = 1;
    getresponse(ftp);
    showresponse(ftp);
    return 0;

}

int CCC(FTP *ftp){
    if(0 != checklogin(ftp)){
        printf("not login ...\n");
        return -1;
    }
    if(!ftp->AUTHMODE){
        printf("not auth yet!\n");
        return -1;
    }
    if(ftp->ccc){
        printf("already cancled command channel protection!\n");
        return -1;
    }

    sendrequst(ftp);

    getresponse(ftp);

    showresponse(ftp);
    if(ftp->code != 200){
        printf("CCC failed!\n");
        return -1;
    }else{
        ftp->ccc = 1;
        return 0;
    }
}