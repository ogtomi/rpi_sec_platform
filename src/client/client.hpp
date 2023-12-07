#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <cstring>
#include "../sockets/c_socket.hpp"
#include "../messages/base_message.hpp"

class Client
{
private:
    CSocket* socket;
    BaseMessage read_msg;

public:
    Client(int domain, int service, int protocol, int port, u_long interface);
    ~Client();
    void launch();
    CSocket* get_socket();

private:
    void do_read();
    void do_write();
};

#endif