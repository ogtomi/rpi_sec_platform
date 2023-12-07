#include "client.hpp"

Client::Client(int domain, int service, int protocol, int port, u_long interface)
{
    socket = new CSocket(domain, service, protocol, port, interface);
}

Client::~Client()
{
    delete socket;
}

CSocket* Client::get_socket()
{
    return socket;
}

void Client::do_read()
{
    read(socket->get_sock(), buffer, BUFF_SIZE);

    std::cout << "MSG read: " << buffer << std::endl;
}

void Client::do_write()
{
    char* hello = "Hello from the client.";

    send(socket->get_sock(), hello, std::strlen(hello), 0);
}

void Client::launch()
{
    while(true)
    {
        do_write();
        do_read();
    }
}

int main()
{
    Client client(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
    client.launch();
}
