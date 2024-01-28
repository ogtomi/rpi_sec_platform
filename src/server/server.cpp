#include "server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bcklg)
{
    socket = new SSocket(domain, service, protocol, port, interface, bcklg);
}

Server::~Server()
{
    delete socket;
}

SSocket* Server::get_socket()
{
    return socket;
}

void Server::do_accept()
{
    struct sockaddr_in address = socket->get_address();
    int addrlen = sizeof(address);

    new_socket = accept(socket->get_sock(), (struct sockaddr*) &address, (socklen_t*) &addrlen);

    if(new_socket < 0)
    {
        perror("Socket fail.");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    else
    {
        if(!do_handshake())
        {
            perror("Handshake fail.");
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "Client connected." << std::endl;
        }
    }
}

bool Server::do_handshake()
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
    // Generate and serialize the key
    skey = crypto.generate_ec_key();
    if(skey == NULL) return false;

    serialized_key = crypto.serialize_key(skey, serialized_key_len);
    if(serialized_key == NULL) return false;

    // Send the key to the client
    server_pubkey_msg.body_length(serialized_key_len);
    std::memcpy(server_pubkey_msg.body(), serialized_key, server_pubkey_msg.body_length());
    server_pubkey_msg.encode_header();
    write(new_socket, server_pubkey_msg.data(), server_pubkey_msg.length());
    
    // Read key from the client
    read(new_socket, client_pubkey_msg.data(), KeyMessage::header_length);

    if(client_pubkey_msg.decode_header())
    {
        read(new_socket, client_pubkey_msg.body(), client_pubkey_msg.body_length());
    }
    else
    {
        return false;
    }

    // Obtain a shared secret using ECDH
    ckey = crypto.deserialize_key((unsigned char*)client_pubkey_msg.body(), client_pubkey_msg.body_length());
    if(ckey == NULL) return false;

    secret = crypto.ecdh(&secret_len, skey, ckey);
    if(secret == NULL) return false;

    // Hash the shared secret with SHA256
    crypto.sha256((char*)secret, secret_len, hashed_secret);

    // Use the first 128 bits as AES-128 key and the second 128 bits as IV
    std::memcpy(aes_128_key, hashed_secret, AES_128_KEY_SIZE);
    std::memcpy(iv, hashed_secret + AES_128_KEY_SIZE, IV_SIZE);

    free(serialized_key);
    free(secret);

    return true;
}

void Server::do_read()
{
    read(new_socket, msg.data(), BaseMessage::hash_length);
    read(new_socket, msg.header(), BaseMessage::header_length);

    if(msg.decode_header())
    {
        read(new_socket, msg.body(), msg.body_length());

        if(msg.check_hash())
        {
            std::cout << "SHA256 hash checked successfully" << std::endl;
            msg.aes_128_cbc_decrypt(aes_128_key, iv);
            std::cout << "C: " << msg.body() << std::endl;
        }
    }
}

void Server::do_write()
{
    msg.aes_128_cbc_encrypt(aes_128_key, iv);
    msg.encode_header();
    msg.encode_hash();
    
    write(new_socket, msg.data(), msg.length());
    std::memset(msg.data(), 0, msg.length());
}

void Server::launch()
{
    std::cout << "Waiting for connection..." << std::endl;
    
    do_accept();

    while(true)
    {
        do_read();
        do_write();
    }
}

int main(int argc, char *argv[])
{
    int port = 80;

    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port " << std::endl;
        std::cout << "PORT 80 IS DEFAULT" << std::endl;
    }
    else
    {
        port = atoi(argv[1]);
        std::cout << "PORT SET TO " << port << std::endl;
    }

    Server server(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10);
    server.launch();   
}