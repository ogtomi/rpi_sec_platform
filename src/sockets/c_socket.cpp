#include "c_socket.hpp"

CSocket::CSocket(int domain, int service, int protocol, int port, u_long interface, struct sockaddr_in server)
    :  MSocket(domain, service, protocol, port, interface)
{
    connection = connect_to_network(get_sock(), server);
    test_connection(connection);
}

int CSocket::connect_to_network(int sock, struct sockaddr_in server)
{
    return connect(sock, (struct sockaddr *) &server, sizeof(server));
}