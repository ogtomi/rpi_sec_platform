#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>

class Crypto
{
public:
    void sha256(char* msg, size_t msg_size, unsigned char* output_buff);
    int aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* ciphertext);
    int aes_128_cbc_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* plaintext);
};

#endif