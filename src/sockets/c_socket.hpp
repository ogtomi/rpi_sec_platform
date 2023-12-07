#ifndef _CONNECTING_SOCKET_H_
#define _CONNECTING_SOCKET_H_

#include "./m_socket.hpp"

class CSocket : public MSocket
{
private:
    int connection;
    
public:
    CSocket(int domain, int service, int protocol, int port, u_long interface);
    virtual ~CSocket(){};
    int connect_to_network(int sock, struct sockaddr_in address);
};

#endif