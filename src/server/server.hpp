#ifndef _SERVER_H_
#define _SERVER_H_

#include <unistd.h>
#include <cstring>

#include "../sockets/s_socket.hpp"
#include "../messages/base_message.hpp"
#include "../messages/key_message.hpp"
#include "../user_interface/user_interface.hpp"

class Server
{
private:
    int new_socket;
    SSocket* socket;
    BaseMessage read_msg, write_msg;
    Crypto crypto;
    UserInterface ui;

    enum { AES_128_KEY_SIZE = 16};
    enum { IV_SIZE = 16};
    unsigned char aes_128_key[AES_128_KEY_SIZE] = {'\0'};
    unsigned char iv[IV_SIZE] = {'\0'};

public:
    Server(int domain, int service, int protocol, int port, u_long interface, int bcklg);
    ~Server();
    void launch();
    SSocket* get_socket();

private:
    void do_accept();
    bool do_handshake();
    void do_read();
    void do_write();
};

#endif