#include "server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int bcklg)
{
    socket = new ListeningSocket(domain, service, protocol, port, interface, bcklg);
}

ListeningSocket* Server::get_socket()
{
    return socket;
}

void Server::accepter()
{
    struct sockaddr_in address = socket->get_address();
    int addrlen = sizeof(address);

    new_socket = accept(socket->get_sock(), (struct sockaddr*) &address, (socklen_t*) &addrlen);

    read(new_socket, buffer, BUFF_SIZE);
}

void Server::handler()
{
    std::cout << buffer << std::endl;
}

void Server::responder()
{
    char* hello = "Hello from the server";

    write(new_socket, hello, std::strlen(hello));
    close(new_socket);
}

void Server::launch()
{
    while(true)
    {
        std::cout << "======= WAITING =======" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "======= DONE =======" << std::endl;
    }
}

int main()
{
    Server server(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    server.launch();   
}