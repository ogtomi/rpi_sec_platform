#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <cstring>
#include <thread>

#include "../sockets/c_socket.hpp"
#include "../messages/base_message.hpp"

class Client
{
private:
    CSocket* socket;
    BaseMessage read_msg;

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
    Client(int domain, int service, int protocol, int port, u_long interface);
    ~Client();
    void launch();
    void do_write(const char* message);
    CSocket* get_socket();

private:
    void do_read();
};

#endif