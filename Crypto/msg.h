//
// Created by kyrie on 2024/1/2.
//

#ifndef FTPS1_0_MSG_H
#define FTPS1_0_MSG_H

#define BUFSIZE         512
#define AES_BLOCK_SIZE  16
#define AES_KEY_128     16
#define AES_IV_128      12
#define TAGSIZE         16
#define ADDSIZE         32

enum MSG_TYPE {
    PUBLIC_KEY,
    SECRET_KEY,
    PUBLIC_KEY_REQUEST,
    IV,
    ENCRYPTED_KEY,
    ENCRYPTED_MSG,
};

typedef struct _APP_MSG {
    int type;
    unsigned char payload[BUFSIZE + AES_BLOCK_SIZE];
    int msg_len;
} APP_MSG;


#endif //FTPS1_0_MSG_H
