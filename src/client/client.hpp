#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <cstring>
#include <thread>

#include "../sockets/c_socket.hpp"
#include "../messages/base_message.hpp"
#include "../messages/key_message.hpp"

class Client
{
private:
    CSocket* socket;
    BaseMessage read_msg;
    Crypto crypto;

    enum { AES_128_KEY_SIZE = 16};
    enum { IV_SIZE = 16};
    unsigned char aes_128_key[AES_128_KEY_SIZE] = {'\0'};
    unsigned char iv[IV_SIZE] = {'\0'};

public:
    Client(int domain, int service, int protocol, int port, u_long interface);
    ~Client();
    void launch();
    void do_write(const char* message);
    CSocket* get_socket();

private:
    bool do_handshake();
    void do_read();
};

#endif