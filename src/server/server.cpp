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
    read(new_socket, buffer, BUFF_SIZE);
    std::cout << "C: " << buffer << std::endl;
}

void Server::do_write()
{
    char* hello = "Hello from the server";

    write(new_socket, hello, std::strlen(hello));
}

void Server::launch()
{
    std::cout << "Waiting for connection..." << std::endl;
    
    while(true)
    {
        do_accept();
        do_read();
        do_write();
    }
}

int main()
{
    Server server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    server.launch();   
}