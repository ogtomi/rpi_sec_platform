
#ifndef _SERVER_H_
#define _SERVER_H_

#include <unistd.h>
#include <cstring>
#include "../sockets/listening_socket.hpp"

class Server
{
private:
    enum { BUFF_SIZE = 30000};
    char buffer[BUFF_SIZE] = {0};
    int new_socket;
    ListeningSocket* socket;

public:
    Server(int domain, int service, int protocol, int port, u_long interface, int bcklg);
    void launch();
    ListeningSocket* get_socket();

private:
    void accepter();
    void handler();
    void responder();
};

#endif