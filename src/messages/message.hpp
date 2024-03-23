#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../crypto/crypto.hpp"

class Message
{
public:
    enum { header_length = 4,
            max_body_length = 1024};

protected:
    char data_[header_length + max_body_length] = {'\0'};
    std::size_t body_length_;
    Crypto crypto;

public:
    Message();
    char* data();
    virtual std::size_t length();
    virtual char* body();
    std::size_t body_length();
    void body_length(std::size_t new_length);
    virtual bool decode_header();
    virtual void encode_header();
};

#endif