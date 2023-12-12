#ifndef _KEY_MESSAGE_H_
#define _KEY_MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

class KeyMessage
{
public:
    enum { header_length = 4};
    enum { max_body_length = 512};

private:
    char data_[header_length + max_body_length] = {'\0'};
    std::size_t body_length_;

public:
    KeyMessage();
    char* data();
    std::size_t length();
    char* body();
    std::size_t body_length();
    void body_length(std::size_t new_length);
    bool decode_header();
    void encode_header();
};

#endif