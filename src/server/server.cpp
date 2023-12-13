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
        do_handshake();
        std::cout << "Client connected." << std::endl;
    }
}

void Server::do_handshake()
{
    // Generate and serialize key
    EVP_PKEY *skey = crypto.generate_ec_key();
    size_t serialized_key_len = 0;
    unsigned char* serialized_key = crypto.serialize_key(skey, serialized_key_len);

    // Write key to the client
    KeyMessage server_pubkey_msg;
    server_pubkey_msg.body_length(serialized_key_len);
    std::memcpy(server_pubkey_msg.body(), serialized_key, server_pubkey_msg.body_length());
    server_pubkey_msg.encode_header();
    write(new_socket, server_pubkey_msg.data(), server_pubkey_msg.length());
    
    // READ THE KEY FROM THE CLIENT
    KeyMessage client_pubkey_msg;
    read(new_socket, client_pubkey_msg.data(), KeyMessage::header_length);

    if(client_pubkey_msg.decode_header())
    {
        read(new_socket, client_pubkey_msg.body(), client_pubkey_msg.body_length());
    }

    // ECDH --> shared secret
    EVP_PKEY* ckey = crypto.deserialize_key((unsigned char*)client_pubkey_msg.body(), client_pubkey_msg.body_length());

    size_t secret_len = 0;
    unsigned char* secret = crypto.ecdh(&secret_len, skey, ckey);
    
    // HASH the shared secret with SHA256
    unsigned char hashed_secret[SHA256_DIGEST_LENGTH] = {'\0'};
    crypto.sha256((char*)secret, secret_len, hashed_secret);

    std::cout << "\n";
    for(size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) printf("%02x", hashed_secret[i]);
    std::cout << "\n";
    // READ encrypted AES KEY & IV from the client
    // Decrypt the AES KEY & IV with HASH as a key
}

void Server::do_read()
{
    read(new_socket, msg.data(), BaseMessage::hash_length);
    std::cout << "Hash read: " << msg.data() << std::endl;

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

int main()
{
    Server server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    server.launch();   
}