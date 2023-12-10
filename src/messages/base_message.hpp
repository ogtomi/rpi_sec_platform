#ifndef _BASE_MESSAGE_H_
#define _BASE_MESSAGE_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../crypto/crypto.hpp"

class BaseMessage
{
public:
    enum { header_length = 4};
    enum { hash_length = SHA256_DIGEST_LENGTH * 2};
    enum { max_body_length = 1024};

private:
    char data_[hash_length + header_length + max_body_length] = {'\0'};
    std::size_t body_length_;

    Crypto crypto;

public:
    BaseMessage();
    char* data();
    std::size_t length();
    char* body();
    std::size_t body_length();
    void body_length(std::size_t new_length);
    char* header();
    bool decode_header();
    void encode_header();
    void encode_hash();
    bool check_hash();
};

#endif