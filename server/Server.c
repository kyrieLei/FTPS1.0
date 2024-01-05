
#include <openssl/aes.h>
#include "../Crypto/secure_communication.h"
#include "init_server.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include <dirent.h> // list command

#include <fcntl.h> // up command :  open()

// params.h
#define BUF_SIZE            256
#define MAX_FILE_SIZE       1000
#define MAX_CLNT            256




// command
#define SUCCESS             1
#define FAIL                (-1)




// handle client command
int handle_command(int clnt_sock);                          /* handle client - command        */
void send_encrypted_list_msg(int clnt_sock);                /* list            command        */


// treating message
int msg_split(char* msg, char* command, char* filename1, char* filename2);


static char plaintext[BUFSIZE+AES_BLOCK_SIZE];
static unsigned char key[AES_KEY_128];
static unsigned char iv[AES_IV_128];
static unsigned char additional[ADDSIZE];


int clnt_cnt;


int main() {

    FTP ftp;
    init(&ftp);

    // socket

    /* ================================= */
    /* variable for secure communication */
    int cnt_i;

    for (cnt_i=0; cnt_i<AES_KEY_128; cnt_i++)
        iv[cnt_i] = (unsigned char)cnt_i;
    for (cnt_i=0; cnt_i<ADDSIZE; cnt_i++)
        additional[cnt_i] = (unsigned char) cnt_i;
    /* ================================= */
    ftp.loop(&ftp);


}



void send_encrypted_list_msg(int clnt_sock) {

    DIR *dir;
    struct dirent *ent;
    dir = opendir ("./");
    char work_dir[256];
    getcwd(work_dir, 256);

    SSL_send(clnt_sock, "Working Directory : ", additional, key, iv);
    SSL_send(clnt_sock, work_dir, additional, key, iv);
    SSL_send(clnt_sock, "\nDirectory List : ", additional, key, iv);


    if (dir != NULL) {
        /* print all the files and directories within directory */
        while ( (ent = readdir(dir)) != NULL ) {
            SSL_send(clnt_sock, ent->d_name, additional, key, iv);
        }
        closedir(dir);
    }
    else {
        /* could not open directory */
        printf("NULL dir\n");

    }
    SSL_send(clnt_sock, "eof", additional, key, iv);
}


