#ifndef TLSSUPPORT_H
#define TLSSUPPORT_H

#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include "Crypto/msg.h"


#define int_error(msg)  handle_error(__FILE__, __LINE__, msg)
void handle_error(const char *file, int lineno, const char *msg);

int init_OpenSSL(void);

static char plaintext[BUFSIZE+AES_BLOCK_SIZE];
static unsigned char iv[AES_IV_128];
static unsigned char additional[ADDSIZE];
static unsigned char key[AES_KEY_128];


long SSL_send(int sock, char *plaintext, unsigned char *additional, unsigned char *key, unsigned char *iv);
long SSL_receive(int sock, char *plaintext, unsigned char *additional, unsigned char *key, unsigned char *iv, int type);

#endif // TLSSUPPORT_H
