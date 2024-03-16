#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <iostream>
#include <cassert>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/core_names.h>
#include <openssl/param_build.h>

class Crypto
{
public:
    void sha256(const char* msg, size_t msg_size, unsigned char* output_buff);
    int aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* ciphertext);
    int aes_128_cbc_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* plaintext);
    EVP_PKEY* generate_ec_key();
    unsigned char* ecdh(size_t *secret_len, EVP_PKEY *pkey, EVP_PKEY *peerkey);
    unsigned char* serialize_key(EVP_PKEY* pkey, size_t& serialized_pubkey_len);
    EVP_PKEY* deserialize_key(unsigned char* serialized_pubkey, size_t serialized_pubkey_len);
};

#endif