#include "key_message.hpp"

KeyMessage::KeyMessage():
    body_length_(0)
{
}

char* KeyMessage::data()
{
    return data_;
}

std::size_t KeyMessage::length()
{
    return header_length + body_length_;
}

char* KeyMessage::body()
{
    return data_ + header_length;
}

std::size_t KeyMessage::body_length()
{
    return body_length_;
}

void KeyMessage::body_length(std::size_t new_length)
{
    body_length_ = new_length;

    if(body_length_ > max_body_length)
        body_length_ = max_body_length;
}

bool KeyMessage::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);

    if(body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }
    
    return true;
}

void KeyMessage::encode_header()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}