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
    EVP_PKEY *skey = crypto.generate_ec_key();
    // Serialize key
    unsigned char* serialized_key = crypto.serialize_key(skey);

    std::cout << "Serialized key" << std::string(reinterpret_cast<char *>(serialized_key)) << std::endl;
    // WRITE THE KEY TO CLIENT
    // READ THE KEY FROM THE CLIENT
    // ECDH --> shared secret
    // HASH the shared secret with SHA256
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