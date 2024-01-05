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

    rsaes_setup_process(ftp.socket_cmd, key, rsa_pubkey, NULL, CLIENT_SIDE);


    ftp.loop(&ftp);

    close(ftp.socket_cmd);
    return 0;

}




