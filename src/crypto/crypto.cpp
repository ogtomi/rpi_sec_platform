#include "crypto.hpp"

void Crypto::sha256(char* msg, size_t msg_size, unsigned char* output_buff)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    const EVP_MD *md = EVP_MD_fetch(NULL, "SHA256", NULL);

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, msg, msg_size);
    EVP_DigestFinal_ex(mdctx, output_buff, 0);

    EVP_MD_CTX_destroy(mdctx);
}

int Crypto::aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        std::cout << "Error during initialization of the context for AES-128-CBC encryption" << std::endl;
    }

    if(EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_128_key, iv) != 1)
    {
        std::cout << "Error during initialization of the AES-128-CBC encryption" << std::endl;
    }

    if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
    {
        std::cout << "Error during encryption update for AES-128-CBC" << std::endl;
    }

    ciphertext_len = len;

    if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
    {
        std::cout << "Error during encryption finalization of the AES-128-CBC" << std::endl;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int Crypto::aes_128_cbc_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        std::cout << "Errorr during the initalization of the context for AES-128-CBC decryption" << std::endl;
    }

    if(EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_128_key, iv) != 1)
    {
        std::cout << "Error during the initialization of the AES-128-CBC decryption" << std::endl;
    }

    if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
    {
        std::cout  << "Error during the decryption update for the AES-128-CBC" << std::endl;
    }

    plaintext_len = len;

    if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
    {
        std::cout << "Error during the decryption finalization ofo the AES-128-CBC" << std::endl;
    }

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}