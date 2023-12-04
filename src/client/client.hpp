#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <cstring>
#include "../sockets/connecting_socket.hpp"

class Client
{
private:
    enum { BUFF_SIZE = 30000};
    char buffer[BUFF_SIZE] = {0};
    int new_socket;
    ConnectingSocket* socket;

public:
    Client(int domain, int service, int protocol, int port, u_long interface);
    void launch();
    ConnectingSocket* get_socket();

private:
    void do_read();
    void do_write();
};

#endif