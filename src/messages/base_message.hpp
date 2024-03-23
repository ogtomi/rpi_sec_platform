#ifndef _BASE_MESSAGE_H_
#define _BASE_MESSAGE_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "message.hpp"
#include "../crypto/crypto.hpp"

class BaseMessage : public Message
{
public:
    enum { hash_length = SHA256_DIGEST_LENGTH * 2};

public:
    BaseMessage();
    std::size_t length() override;
    char* body() override;
    char* header();
    bool decode_header() override;
    void encode_header() override;
    void encode_hash();
    bool check_hash();

    void aes_128_cbc_encrypt(unsigned char* aes_128_key, unsigned char* iv);
    void aes_128_cbc_decrypt(unsigned char* aes_128_key, unsigned char* iv);
};

#endif