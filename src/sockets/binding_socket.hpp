#ifndef _BINDING_SOCKET_H_
#define _BINDING_SOCKET_H_

#include "./m_socket.hpp"

class BindingSocket : public MSocket
{
public:
    BindingSocket(int domain, int service, int protocol, int port, u_long interface);
    int connect_to_network(int sock, struct sockaddr_in address);
};

#endif
