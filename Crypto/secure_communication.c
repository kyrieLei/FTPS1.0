//
// Created by kyrie on 2024/1/2.
//

#include "secure_communication.h"

#include "gcm_encrypto.h"
#include "../IO.h"
#include <openssl/pem.h>
#include <openssl/rsa.h>

APP_MSG msg_in, msg_out;

int rsaes_setup_process(int sock, unsigned char *key, BIO *rsa_pubkey, BIO *rsa_privkey, int side) {


    // setup process - server side
    if ( side == SERVER_SIDE )
    {
        int n;
        int publickey_len;
        int encryptedkey_len;
        BIO *pub = NULL;
        unsigned char buffer[BUFSIZE] = { 0x00, };

        memset(&msg_in, 0, sizeof(APP_MSG)); // msg_out

        Readn(sock, &msg_in, sizeof(APP_MSG)); // 공개키 요청 메시지 수신
        msg_in.type = ntohl(msg_in.type);
        msg_in.msg_len = ntohl(msg_in.msg_len);


        if ( msg_in.type != PUBLIC_KEY_REQUEST )
            printf("message error (PUBLIC_KEY_REQUEST)");
        else {
            // sending PUBLIC_KEY
            memset(&msg_out, 0, sizeof(msg_out));
            msg_out.type = PUBLIC_KEY;
            msg_out.type = htonl(msg_out.type);

            pub = BIO_new(BIO_s_mem());
            PEM_write_bio_RSAPublicKey(pub, rsa_pubkey);
            publickey_len = BIO_pending(pub);

            BIO_read(pub, msg_out.payload, publickey_len);
            msg_out.msg_len = htonl(publickey_len);

            Writen(sock, &msg_out, sizeof(APP_MSG)); // clnt_sock

        }

        // 클라이언트로부터 암호화된 세션키 수신, 복호화하여 세션키 복원
        memset(&msg_in, 0, sizeof(APP_MSG)); // msg_out
        Readn(sock, &msg_in, sizeof(APP_MSG));

        msg_in.type = ntohl(msg_in.type);
        msg_in.msg_len = ntohl(msg_in.msg_len);

        if ( msg_in.type != ENCRYPTED_KEY ) {
            printf("message error (ENCRYPTED_KEY)");
        }
        else {
            encryptedkey_len = RSA_private_decrypt(msg_in.msg_len, msg_in.payload, buffer, rsa_privkey, RSA_PKCS1_OAEP_PADDING);
            memcpy(key, buffer, encryptedkey_len);

        }
    }

        // setup process - client side
    else if ( side == CLIENT_SIDE )
    {
        int n;
        BIO *rpub = NULL;


        memset(&msg_out, 0, sizeof(msg_out));
        msg_out.type = PUBLIC_KEY_REQUEST;
        msg_out.type = htonl(msg_out.type);



        Writen(sock, &msg_out, sizeof(APP_MSG)); //serv_sock



        memset(&msg_in, 0, sizeof(APP_MSG)); // msg_out

        Readn(sock, &msg_in, sizeof(APP_MSG));



        msg_in.type = ntohl(msg_in.type);
        msg_in.msg_len = ntohl(msg_in.msg_len);


        if (msg_in.type != PUBLIC_KEY)
            printf("message error");
        else {

            // BIO_dump_fp(stdout, (const char*)msg_in.payload, msg_in.msg_len);
            rpub = BIO_new_mem_buf(msg_in.payload, -1);
            BIO_write(rpub, msg_in.payload, msg_in.msg_len); // rpub로 공개키 읽어들이기
            if ( !PEM_read_bio_RSAPublicKey(rpub, &rsa_pubkey, NULL, NULL) )
                printf("PEM_read_bio_RSAPublicKey() error");

        }


        // sending ENCRYPTED_KEY msg (공개키로 암호화된 비밀키 송신)
        memset(&msg_out, 0, sizeof(APP_MSG));
        msg_out.type = ENCRYPTED_KEY;
        msg_out.type = htonl(msg_out.type);
        msg_out.msg_len = RSA_public_encrypt(AES_KEY_128, key, msg_out.payload, rsa_pubkey, RSA_PKCS1_OAEP_PADDING);
        msg_out.msg_len = htonl(msg_out.msg_len);


        Writen(sock, &msg_out, sizeof(APP_MSG));

    }
    else {
        printf("check if side isn't `SERVER` or `CLIENT` ");
    }
}



