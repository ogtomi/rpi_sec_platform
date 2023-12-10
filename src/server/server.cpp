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
            std::cout << "C: " << msg.body() << std::endl;
        }
    }
}

void Server::do_write()
{
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