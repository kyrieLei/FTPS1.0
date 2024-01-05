//
// Created by kyrie on 2024/1/2.
//

#ifndef FTPS_SECURE_COMMUNICATION_H
#define FTPS_SECURE_COMMUNICATION_H


#ifndef __SECURE_COMMUNICATION__
#define __SECURE_COMMUNICATION__

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include "msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <assert.h>

#define SERVER_SIDE 10000
#define CLIENT_SIDE 20000
#define CLIENT_LIST 20001

#define MODE

// SIDE :
//     - SERVER : send rsa pubkey and receive aes session key
//     - CLIENT : receive rsa pubkey and send aes session key
int rsaes_setup_process(int sock, unsigned char *key, BIO *rsa_pubkey, BIO *rsa_privkey, int side);




#endif


#endif //FTPS_SECURE_COMMUNICATION_H
