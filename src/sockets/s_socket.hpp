#ifndef _BINDING_SOCKET_H_
#define _BINDING_SOCKET_H_

#include "./m_socket.hpp"

class SSocket : public MSocket
{
private:
    int binding;
    int backlog;
    int listening;
    
public:
    SSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);
    virtual ~SSocket(){};
    int connect_to_network(int sock, struct sockaddr_in address);
    int start_listening();
};

#endif
