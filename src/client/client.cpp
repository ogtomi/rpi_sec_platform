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
    read(socket->get_sock(), read_msg.header(), BaseMessage::header_length);

    if(read_msg.decode_header())
    {
        read(socket->get_sock(), read_msg.body(), read_msg.body_length());
        std::cout << "S: " << read_msg.body() << std::endl;
    }
}

void Client::do_write()
{
    BaseMessage write_msg;

    char* hello = "Hello from the client.";

    write_msg.body_length(std::strlen(hello));
    std::memcpy(write_msg.body(), hello, write_msg.body_length());
    write_msg.encode_header();

    send(socket->get_sock(), write_msg.data(), write_msg.length(), 0);
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
