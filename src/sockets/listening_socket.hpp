#ifndef _LISTENING_SOCKET_H_
#define _LISTENING_SOCKET_H_

#include "binding_socket.hpp"

class ListeningSocket : public BindingSocket
{
private:
    int backlog;
    int listening;

public:
    ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bcklg);
    void start_listening();
};

#endif