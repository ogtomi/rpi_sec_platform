#include "crypto.hpp"

void Crypto::sha256(const char* msg, size_t msg_size, unsigned char* output_buff)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    const EVP_MD *md = EVP_MD_fetch(NULL, "SHA256", NULL);

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, msg, msg_size);
    EVP_DigestFinal_ex(mdctx, output_buff, NULL);

    EVP_MD_CTX_destroy(mdctx);
}

void Crypto::sha256_to_char(const unsigned char* msg, size_t msg_size, char* output_buff)
{
    char *ptr = &output_buff[0];
    
    for(size_t i = 0; i < msg_size; i++)
    {
        ptr += std::sprintf(ptr, "%02x", msg[i]);
    }
}

int Crypto::aes_128_cbc_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        std::cout << "Error during initialization of the context for AES-128-CBC encryption" << std::endl;
        return -1;
    }

    if(EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_128_key, iv) != 1)
    {
        std::cout << "Error during initialization of the AES-128-CBC encryption" << std::endl;
        return -1;
    }

    if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
    {
        std::cout << "Error during encryption update for AES-128-CBC" << std::endl;
        return -1;
    }

    ciphertext_len = len;

    if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
    {
        std::cout << "Error during encryption finalization of the AES-128-CBC" << std::endl;
        return -1;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int Crypto::aes_128_cbc_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* aes_128_key, unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        std::cout << "Errorr during the initalization of the context for AES-128-CBC decryption" << std::endl;
        return -1;
    }

    if(EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, aes_128_key, iv) != 1)
    {
        std::cout << "Error during the initialization of the AES-128-CBC decryption" << std::endl;
        return -1;
    }

    if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
    {
        std::cout  << "Error during the decryption update for the AES-128-CBC" << std::endl;
        return -1;
    }

    plaintext_len = len;

    if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
    {
        std::cout << "Error during the decryption finalization ofo the AES-128-CBC" << std::endl;
        return -1;
    }

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

EVP_PKEY* Crypto::generate_ec_key()
{
    EVP_PKEY_CTX *ctx;
    EVP_PKEY *pkey = NULL;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);

    if(ctx == NULL)
        goto err;
    
    if(EVP_PKEY_keygen_init(ctx) <= 0)
        goto err;

    if(EVP_PKEY_CTX_set_ec_paramgen_curve_nid(ctx, NID_X9_62_prime256v1) <= 0)
        goto err;

    if(EVP_PKEY_keygen(ctx, &pkey) <= 0)
        goto err;

    EVP_PKEY_CTX_free(ctx);
    return pkey;

    err:
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        return NULL;
}

unsigned char* Crypto::ecdh(size_t *secret_len, EVP_PKEY *pkey, EVP_PKEY *peerkey)
{
    EVP_PKEY_CTX *ctx = NULL;
    unsigned char *secret;

    if((ctx = EVP_PKEY_CTX_new(pkey, NULL)) == NULL)
    {
        std::cout << "Error during creation of the context for the shared sacred derivation" << std::endl;
        return NULL;
    }

    if(EVP_PKEY_derive_init(ctx) != 1)
    {
        std::cout << "Error during initialisation" << std::endl;
        return NULL;
    }

    if(EVP_PKEY_derive_set_peer(ctx, peerkey) != 1)
    {
        std::cout << "Error during providing the peer public key" << std::endl;
        return NULL;
    }

    if(EVP_PKEY_derive(ctx, NULL, secret_len) != 1)
    {
        std::cout << "Error during determining the buffer length for shared secret" << std::endl;
        return NULL;
    }

    if((secret = (unsigned char*)OPENSSL_malloc(*secret_len)) == NULL)
    {
        std::cout << "Error during creation of the buffer" << std::endl;
        return NULL;
    }

    if((EVP_PKEY_derive(ctx, secret, secret_len)) != 1)
    {
        std::cout << "Error during derivation of the secret key" << std::endl;
        return NULL;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pkey);
    EVP_PKEY_free(peerkey);

    return secret;
}

unsigned char* Crypto::serialize_key(EVP_PKEY* pkey, size_t& serialized_pubkey_len)
{
    if(EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PUB_KEY, NULL, 0, &serialized_pubkey_len) != 1)
    {
        std::cout << "Error during get octet string param" << std::endl;
        return NULL;
    }

    unsigned char* serialized_pubkey = (unsigned char*) OPENSSL_malloc(serialized_pubkey_len);

    if(EVP_PKEY_get_octet_string_param(pkey, OSSL_PKEY_PARAM_PUB_KEY, serialized_pubkey, serialized_pubkey_len, &serialized_pubkey_len) != 1)
    {
        std::cout << "Error during get the serialized public key" << std::endl;
        return NULL;
    }

    return serialized_pubkey;
}

EVP_PKEY* Crypto::deserialize_key(unsigned char* serialized_pubkey, size_t serialized_pubkey_len)
{
    OSSL_PARAM_BLD* param_build = OSSL_PARAM_BLD_new();

    if(param_build == NULL)
    {
        OSSL_PARAM_BLD_free(param_build);
        return NULL;
    }

    if(OSSL_PARAM_BLD_push_utf8_string(param_build, OSSL_PKEY_PARAM_GROUP_NAME, "prime256v1", 0) != 1)
    {
        OSSL_PARAM_BLD_free(param_build);
        return NULL;
    }
    
    if(OSSL_PARAM_BLD_push_octet_string(param_build, OSSL_PKEY_PARAM_PUB_KEY, serialized_pubkey, serialized_pubkey_len) != 1)
    {
        OSSL_PARAM_BLD_free(param_build);
        return NULL;
    }
    
    OSSL_PARAM* params = OSSL_PARAM_BLD_to_param(param_build);

    if(params == NULL)
    {
        OSSL_PARAM_BLD_free(param_build);
        return NULL;
    }

    EVP_PKEY_CTX* pubkey_ctx = EVP_PKEY_CTX_new_from_name(NULL, "EC", NULL);
    if(pubkey_ctx == NULL)
    {
        OSSL_PARAM_BLD_free(param_build);
        OSSL_PARAM_free(params);
        return NULL;
    }

    if(EVP_PKEY_fromdata_init(pubkey_ctx) <= 0)
    {
        OSSL_PARAM_BLD_free(param_build);
        OSSL_PARAM_free(params);
        EVP_PKEY_CTX_free(pubkey_ctx);
        return NULL;
    }

    EVP_PKEY* pubkey = NULL;
    
    if(EVP_PKEY_fromdata(pubkey_ctx, &pubkey, EVP_PKEY_PUBLIC_KEY, params) <= 0)
    {
        OSSL_PARAM_BLD_free(param_build);
        OSSL_PARAM_free(params);
        EVP_PKEY_CTX_free(pubkey_ctx);
        return NULL;
    }

    OSSL_PARAM_BLD_free(param_build);
    OSSL_PARAM_free(params);
    EVP_PKEY_CTX_free(pubkey_ctx);

    return pubkey;
}
