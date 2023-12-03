#include "sockets/listening_socket.hpp"

int main()
{
    std::cout << "Starting..." << std::endl;
    std::cout << "Binding Socket..." << std::endl;
    int option = 1;

    BindingSocket bs = BindingSocket(AF_INET, SOCK_STREAM, 0, 6600, INADDR_ANY);

    std::cout << "Listening Socket..." << std::endl;

    ListeningSocket ls = ListeningSocket(AF_INET, SOCK_STREAM, 0, 6601, INADDR_ANY, 10);

    std::cout << "Success" << std::endl;
}