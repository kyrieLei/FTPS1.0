#include <unistd.h>
#include "tls.h"
#include "Crypto/gcm_encrypto.h"
#include "IO.h"
#include "Crypto/secure_communication.h"


APP_MSG msg_in, msg_out;

void handle_error(const char *file, int lineno, const char *msg)
{
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
}

long SSL_send(int sock, char *plaintext, unsigned char *additional, unsigned char *key, unsigned char *iv) {

    int len, plaintext_len, ciphertext_len, n;
    unsigned char tag[TAGSIZE] = { 0x00, };



    len = strlen(plaintext);
//    if ( plaintext[len-1] == '\n' )
//        plaintext[len-1] = '\0';

    if ( strlen(plaintext) == 0 )
        return 0;

    memset(&msg_out, 0, sizeof(msg_out));
    msg_out.type = ENCRYPTED_MSG;
    msg_out.type = htonl(msg_out.type);
    ciphertext_len = gcm_encrypt((unsigned char*)plaintext, len, additional, strlen((char*)additional), key, iv, msg_out.payload, tag);
    msg_out.msg_len = htonl(ciphertext_len);

    Writen(sock, &msg_out, sizeof(APP_MSG));

    Writen(sock, tag, 16);
    return 0;


}

long SSL_receive(int sock, char *plaintext, unsigned char *additional, unsigned char *key, unsigned char *iv, int type) {

    int n, plaintext_len;
    unsigned char tag[TAGSIZE] = { 0x00, };

    Readn(sock, &msg_in, sizeof(APP_MSG));

    Readn(sock, tag, 16);

    msg_in.type = ntohl(msg_in.type);
    msg_in.msg_len = ntohl(msg_in.msg_len);
    switch(msg_in.type) {
        case ENCRYPTED_MSG:
            plaintext_len = gcm_decrypt(msg_in.payload, msg_in.msg_len, additional, strlen((char*)additional), key, iv, (unsigned char*)plaintext, tag);
            if ( type == CLIENT_LIST ) {
                plaintext[plaintext_len] = '\0';
                if ( strcmp(plaintext, "eof") )
                    printf("%s", plaintext);
            }
            break;
        default:
            break;
    }

    return plaintext_len;

}


