#include "base_message.hpp"

BaseMessage::BaseMessage():
    body_length_(0)
{
}

char* BaseMessage::data()
{
    return data_;
}

std::size_t BaseMessage::length()
{
    return header_length + body_length_;
}

char* BaseMessage::body()
{
    return data_ + header_length;
}

std::size_t BaseMessage::body_length()
{
    return body_length_;
}

void BaseMessage::body_length(std::size_t new_length)
{
    body_length_ = new_length;

    if(body_length_ > max_body_length)
        body_length_ = max_body_length;
}

char* BaseMessage::header()
{
    return data_;
}

bool BaseMessage::decode_header()
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

void BaseMessage::encode_header()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}