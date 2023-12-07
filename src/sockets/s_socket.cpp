#include "s_socket.hpp"

SSocket::SSocket(int domain, int service, int protocol, int port, u_long interface, int bklg)
    :  MSocket(domain, service, protocol, port, interface)
{
    binding = connect_to_network(get_sock(), get_address());
    test_connection(binding);

    listening = start_listening();
    test_connection(listening);
}

int SSocket::connect_to_network(int sock, struct sockaddr_in address)
{
    return bind(sock, (struct sockaddr *) &address, sizeof(address));
}

int SSocket::start_listening()
{
    return listen(get_sock(), backlog);
}