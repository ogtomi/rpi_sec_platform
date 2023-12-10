#include "crypto.hpp"

void Crypto::sha256(char* msg, size_t msg_size, unsigned char* output_buff)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    const EVP_MD *md = EVP_MD_fetch(NULL, "SHA256", NULL);
    unsigned char md_value[EVP_MAX_MD_SIZE];

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, msg, msg_size);
    EVP_DigestFinal_ex(mdctx, output_buff, 0);

    EVP_MD_CTX_destroy(mdctx);
}