#ifndef _MSOCKET_H_
#define _MSOCKET_H_

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

class MSocket
{
private:
    struct sockaddr_in address;
    int sock;
    int option = 1;

public:
    MSocket(int domain, int service, int protocol, int port, u_long interface);
    virtual ~MSocket(){};
    virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
    void test_connection(int test_item);

    struct sockaddr_in get_address();
    int get_sock();
};

#endif