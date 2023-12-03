#include "listening_socket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bcklg)
    : BindingSocket(domain, service, protocol, port, interface)
{
    backlog = bcklg;
    start_listening();
    test_connection(listening);
}

void ListeningSocket::start_listening()
{
    listening = listen(get_sock(), backlog);
}