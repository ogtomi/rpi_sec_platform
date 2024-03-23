#include "base_message.hpp"

BaseMessage::BaseMessage():
    Message()
{
}

std::size_t BaseMessage::length()
{
    return hash_length + header_length + body_length_;
}

char* BaseMessage::body()
{
    return data_ + header_length + hash_length;
}

char* BaseMessage::header()
{
    return data_ + hash_length;
}

bool BaseMessage::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, data_ + hash_length, header_length);
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
    std::memcpy(data_ + hash_length, header, header_length);
}

void BaseMessage::encode_hash()
{
    unsigned char hash[SHA256_DIGEST_LENGTH + 1];
    char hash_buff[hash_length + 1];

    crypto.sha256(this->body(), std::strlen(this->body()), hash);
    crypto.sha256_to_char(hash, SHA256_DIGEST_LENGTH, hash_buff);

    std::memcpy(data_, hash_buff, hash_length);
}

bool BaseMessage::check_hash()
{
    unsigned char hash_calculated[SHA256_DIGEST_LENGTH + 1];
    char hash_calculated_buff[hash_length + 1];

    crypto.sha256(this->body(), std::strlen(this->body()), hash_calculated);
    crypto.sha256_to_char(hash_calculated, SHA256_DIGEST_LENGTH, hash_calculated_buff);

    for(int i = 0; i < hash_length; i++)
    {
        if(*(this->data() + i) != hash_calculated_buff[i])
            return false;
    }

    return true;
}

void BaseMessage::aes_128_cbc_encrypt(unsigned char* aes_128_key, unsigned char* iv)
{
    char ciphertext_buff[max_body_length + 1] = {'\0'};
    int ciphertext_len = crypto.aes_128_cbc_encrypt(reinterpret_cast<unsigned char*> (this->body()),
                                                    this->body_length(),
                                                    aes_128_key,
                                                    iv,
                                                    reinterpret_cast<unsigned char*> (ciphertext_buff));

    std::memset(this->data(), 0, this->length());
    this->body_length(ciphertext_len);
    std::memcpy(this->body(), ciphertext_buff, this->body_length());
}

void BaseMessage::aes_128_cbc_decrypt(unsigned char* aes_128_key, unsigned char* iv)
{
    char plaintext_buff[max_body_length + 1] = {'\0'};
    int plaintext_len = crypto.aes_128_cbc_decrypt(reinterpret_cast<unsigned char*> (this->body()),
                                                    this->body_length(),
                                                    aes_128_key,
                                                    iv,
                                                    reinterpret_cast<unsigned char*> (plaintext_buff));

    std::memset(this->data(), 0, this->length());
    this->body_length(plaintext_len);
    std::memcpy(this->body(), plaintext_buff, this->body_length());
}