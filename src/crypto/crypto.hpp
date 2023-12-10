#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <openssl/evp.h>
#include <openssl/sha.h>

class Crypto
{
public:
    void sha256(char* msg, size_t msg_size, unsigned char* output_buff);
};

#endif