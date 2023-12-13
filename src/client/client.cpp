#include "client.hpp"

Client::Client(int domain, int service, int protocol, int port, u_long interface)
{
    socket = new CSocket(domain, service, protocol, port, interface);

    do_handshake();
}

Client::~Client()
{
    delete socket;
}

CSocket* Client::get_socket()
{
    return socket;
}

void Client::do_handshake()
{   
    // Client & Server keys (+ key messages)
    EVP_PKEY *ckey = NULL, *skey = NULL;
    KeyMessage server_pubkey_msg, client_pubkey_msg;

    // Key serialization vars
    size_t serialized_key_len = 0;
    unsigned char* serialized_key = NULL;
    
    // Shared secret (ECDH)
    unsigned char* secret = NULL;
    size_t secret_len = 0;
    unsigned char hashed_secret[SHA256_DIGEST_LENGTH] = {'\0'};
    
    // HANDSHAKE
    // Generate and serialize key
    ckey = crypto.generate_ec_key();
    serialized_key = crypto.serialize_key(ckey, serialized_key_len);

    // Read key from the server
    read(socket->get_sock(), server_pubkey_msg.data(), KeyMessage::header_length);

    if(server_pubkey_msg.decode_header())
    {
        read(socket->get_sock(), server_pubkey_msg.body(), server_pubkey_msg.body_length());
    }

    // Send the key to the server
    client_pubkey_msg.body_length(serialized_key_len);
    std::memcpy(client_pubkey_msg.body(), serialized_key, client_pubkey_msg.body_length());
    client_pubkey_msg.encode_header();
    write(socket->get_sock(), client_pubkey_msg.data(), client_pubkey_msg.length());

    // Obtain a shared secret using ECDH
    skey = crypto.deserialize_key((unsigned char*)server_pubkey_msg.body(), server_pubkey_msg.body_length());
    secret = crypto.ecdh(&secret_len, ckey, skey);
    
    // Hash the shared secret with SHA256
    crypto.sha256((char*)secret, secret_len, hashed_secret);

    // Use the first 128 bytes as AES-128 key and the second 128 bytes as IV
    std::memcpy(aes_128_key, hashed_secret, AES_128_KEY_SIZE);
    std::memcpy(iv, hashed_secret + AES_128_KEY_SIZE, IV_SIZE);

    EVP_PKEY_free(ckey);
    EVP_PKEY_free(skey);
    free(serialized_key);
    free(secret);
}

void Client::do_read()
{
    read(socket->get_sock(), read_msg.data(), BaseMessage::hash_length);
    read(socket->get_sock(), read_msg.header(), BaseMessage::header_length);

    if(read_msg.decode_header())
    {
        read(socket->get_sock(), read_msg.body(), read_msg.body_length());

        if(read_msg.check_hash())
        {
            std::cout << "SHA256 hash checked successfully" << std::endl;
            read_msg.aes_128_cbc_decrypt(aes_128_key, iv);
            std::cout << "S: " << read_msg.body() << std::endl;
        }
    }

    std::memset(read_msg.data(), 0, read_msg.length());
}

void Client::do_write(const char* message)
{
    BaseMessage write_msg;
    write_msg.body_length(std::strlen(message));
    std::memcpy(write_msg.body(), message, write_msg.body_length());
    
    write_msg.aes_128_cbc_encrypt(aes_128_key, iv);
    write_msg.encode_header();
    write_msg.encode_hash();

    send(socket->get_sock(), write_msg.data(), write_msg.length(), 0);
}

void Client::launch()
{
    while(true)
    {
        do_read();
    }
}

int main()
{
    Client client(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
    
    std::thread t(
        [&client](){
            client.launch();
        }
    );

    char line[BaseMessage::max_body_length + 1] = {'\0'};

    while(std::cin.getline(line, BaseMessage::max_body_length + 1))
    {
        client.do_write(line);
    }

    t.join();
}
