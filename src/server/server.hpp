#ifndef _SERVER_H_
#define _SERVER_H_

#include <unistd.h>
#include <cstring>
#include "../sockets/s_socket.hpp"
#include "../messages/base_message.hpp"

class Server
{
private:
    int new_socket;
    SSocket* socket;
    BaseMessage msg;

public:
    Server(int domain, int service, int protocol, int port, u_long interface, int bcklg);
    ~Server();
    void launch();
    SSocket* get_socket();

private:
    void do_accept();
    void do_read();
    void do_write();
};

#endif