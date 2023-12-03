#include "m_socket.hpp"

MSocket::MSocket(int domain, int service, int protocol, int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);

    sock = socket(domain, service, protocol);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    
    test_connection(sock);
}

void MSocket::test_connection(int test_item)
{
    if(test_item < 0)
    {
        perror("Failed to connect.");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in MSocket::get_address()
{
    return address;
}

int MSocket::get_sock()
{
    return sock;
}

int MSocket::get_connection()
{
    return connection;
}

void MSocket::set_connection(int con)
{
    connection = con;
}