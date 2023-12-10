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

    // CRYPTO - DEV
    enum { AES_128_KEY_SIZE = 16};
    enum { IV_SIZE = 16};

    unsigned char aes_128_key[AES_128_KEY_SIZE] = { 0x00, 0x01, 0x02, 0x03,
                                                    0x04, 0x05, 0x06, 0x07,
                                                    0x08, 0x09, 0x0A, 0x0B,
                                                    0x0C, 0x0D, 0x0E, 0X0f};

    unsigned char iv[IV_SIZE] = {   0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00};

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