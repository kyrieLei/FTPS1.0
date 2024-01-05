/*
    Secure FTP Project (c Yongbhin Kim)
        - FTP application program
        - Client
*/



#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/rand.h>



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h> 
#include "../Crypto/msg.h"
#include "../Crypto/secure_communication.h"
#include "init_client.h"



// params.h
#define BUF_SIZE            256




void handle_command(FTP* ftp,int sock);

void send_up_file(FTP*ftp,int sock, char* src_file);
void recv_down_file(FTP*ftp,int sock, char *filename2);

void msg_split(char* msg, char* command, char* filename1, char* filename2);



int main(int argc, char* argv[]) {
    FTP ftp;
    init(&ftp, "127.0.0.1", 9876);

    BIO *rsa_pubkey = NULL;

    int cnt_i;

    RAND_poll();
    RAND_bytes(key, sizeof(key));

    for (cnt_i=0; cnt_i<AES_KEY_128; cnt_i++)
        iv[cnt_i] = (unsigned char) cnt_i;
    for (cnt_i=0; cnt_i<ADDSIZE; cnt_i++)
        additional[cnt_i] = (unsigned char) cnt_i;
    // setup process
    rsaes_setup_process(ftp.socket_cmd, key, rsa_pubkey, NULL, CLIENT_SIDE);

    //handle_command(&ftp,ftp.socket_cmd);
    //printf("感觉良好");
    ftp.loop(&ftp);

    close(ftp.socket_cmd);
    return 0;

}


void handle_command(FTP*ftp,int sock) {

    while (1) {

        printf("\n\n ======================\n");
        printf(" |       Command      |\n");
        printf(" ----------------------\n");
        printf(" list \n");
        printf(" up \n");
        printf(" down \n");
        printf(" q or Q \n");
        printf(" ======================\n");
        printf(" > ");

        char msg[BUFSIZE];
        memset(&msg, 0, BUFSIZE);
        fgets(msg, BUFSIZE, stdin);


        SSL_send(sock, msg, additional, key, iv);


        memset(&plaintext, 0, sizeof(plaintext));
        SSL_receive(sock, plaintext, additional, key, iv, CLIENT_SIDE); // list OR up OR down

        char command[BUF_SIZE], filename1[BUF_SIZE], filename2[BUF_SIZE];
        msg_split(msg, command, filename1, filename2);


        if ( !strcmp(command, "list") || !strcmp(command, "list\n") || !strcmp(command, "list\n ") || !strcmp(command, "list ") ) {
            while ( strcmp(plaintext, "eof") )
                SSL_receive(sock, plaintext, additional, key, iv, CLIENT_LIST);
        }
        else if ( !strcmp(command, "up ") || !strcmp(command, "up") || !strcmp(command, "up\n") ) {
            send_up_file(ftp,sock, filename1);
        }
        else if ( !strcmp(command, "down ") || !strcmp(command, "down") || !strcmp(command, "down\n") ) {
            recv_down_file(ftp,sock, filename2);
        }
        else {
            printf("Error commmand ");
        }

    }

}

// aes-gcm
void recv_down_file(FTP*ftp,int sock, char *filename2) {

    memset(&(plaintext), 0, sizeof(plaintext));
    SSL_receive(sock, plaintext, additional, key, iv, CLIENT_SIDE); // 파일 내용 , CLIENT_SIDE받기

    char tmp[BUFSIZE];
    int fd = open(filename2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    sprintf( tmp, "%s", plaintext);
    write(fd, tmp, strlen(tmp)); // 파일에 쓰기

    close(fd);
}

// aes-gcm
void send_up_file(FTP*ftp,int sock, char* src_file) {
    // file open and send status
    FILE* ptr = fopen(src_file, "rb");

    if (ptr == NULL) {
        printf("File open error\n");
        return;
    }

    while (!feof(ptr)) {
        fgets(plaintext, BUFSIZE, ptr);
        SSL_send(sock, plaintext, additional, key, iv);
        memset(&(plaintext), 0, sizeof(plaintext));
    }
}


void msg_split(char* msg, char* command, char* filename1, char* filename2) {

    if ( !strcmp(msg, "list\n") || !strcmp(msg, "list") ) {
        strcpy(command, msg);
        return;
    }

    char *ptr = strtok(msg, " ");

    if (ptr != NULL)
    {
        strcpy(command, ptr);
        ptr = strtok(NULL, " ");
        if (ptr != NULL) {
            strcpy(filename1, ptr);
            ptr = strtok(NULL, " ");
            strcpy(filename2, ptr);
        }
    }
}
