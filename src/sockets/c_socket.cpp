#include "c_socket.hpp"

CSocket::CSocket(int domain, int service, int protocol, int port, u_long interface)
    :  MSocket(domain, service, protocol, port, interface)
{
    connection = connect_to_network(get_sock(), get_address());
    test_connection(connection);
}

int CSocket::connect_to_network(int sock, struct sockaddr_in address)
{
    return connect(sock, (struct sockaddr *) &address, sizeof(address));
}