//
// Created by kyrie on 2024/1/2.
//

#include "gcm_encrypto.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/conf.h>



int gcm_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *add, int add_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, unsigned char *tag) {
    EVP_CIPHER_CTX *ctx = NULL;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();


    if( 1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, key, iv) )
        handleErrors();

    if( 1 != EVP_EncryptUpdate(ctx, NULL, &len, add, add_len) )
        handleErrors();

    if( 1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) )
        handleErrors();

    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    if ( 1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) )
        handleErrors();

    if( ctx != NULL )
        EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;

}

int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *add, int add_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned char *tag) {

    EVP_CIPHER_CTX *ctx = NULL;
    int len;
    int plaintext_len;
    int ret;

    if ( !(ctx = EVP_CIPHER_CTX_new()) )
        handleErrors();

    if ( 1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, key, iv) )
        handleErrors();

    if ( 1 != EVP_DecryptUpdate(ctx, NULL, &len, add, add_len) )
        handleErrors();

    if ( 1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) )
        handleErrors();
    plaintext_len = len;

    if ( !EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag) )
        handleErrors();

    ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    if ( ctx != NULL)
        EVP_CIPHER_CTX_free(ctx);


    if ( ret > 0) {
        plaintext_len += len;
        return plaintext_len;
    }
    else {
        return -1;
    }
}

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}
