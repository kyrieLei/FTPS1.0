//
// Created by kyrie on 2024/1/2.
//
#pragma once
#ifndef FTPS_GCM_ENCRYPTO_H
#define FTPS_GCM_ENCRYPTO_H





void handleErrors(void);
int gcm_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *add, int add_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, unsigned char *tag);
int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *add, int add_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned char *tag);



#endif //FTPS_GCM_ENCRYPTO_H
