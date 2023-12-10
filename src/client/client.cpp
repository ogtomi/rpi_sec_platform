#include "client.hpp"

Client::Client(int domain, int service, int protocol, int port, u_long interface)
{
    socket = new CSocket(domain, service, protocol, port, interface);
}

Client::~Client()
{
    delete socket;
}

CSocket* Client::get_socket()
{
    return socket;
}

void Client::do_read()
{
    read(socket->get_sock(), read_msg.data(), BaseMessage::hash_length);
    std::cout << "Hash read: " << read_msg.data() << std::endl;

    read(socket->get_sock(), read_msg.header(), BaseMessage::header_length);

    if(read_msg.decode_header())
    {
        read(socket->get_sock(), read_msg.body(), read_msg.body_length());

        if(read_msg.check_hash())
        {
            std::cout << "S: " << read_msg.body() << std::endl;
            std::cout << "SHA256 hash checked successfully" << std::endl;
            read_msg.aes_128_cbc_decrypt(aes_128_key, iv);
            std::cout << "S: " << read_msg.body() << std::endl;
        }
    }

    std::memset(read_msg.data(), 0, read_msg.length());
}

void Client::do_write(const char* message)
{
    BaseMessage write_msg;
    write_msg.body_length(std::strlen(message));
    std::memcpy(write_msg.body(), message, write_msg.body_length());
    
    write_msg.aes_128_cbc_encrypt(aes_128_key, iv);
    write_msg.encode_header();
    write_msg.encode_hash();

    send(socket->get_sock(), write_msg.data(), write_msg.length(), 0);
}

void Client::launch()
{
    while(true)
    {
        do_read();
    }
}

int main()
{
    Client client(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
    
    std::thread t(
        [&client](){
            client.launch();
        }
    );

    char line[BaseMessage::max_body_length + 1] = {'\0'};

    while(std::cin.getline(line, BaseMessage::max_body_length + 1))
    {
        client.do_write(line);
    }

    t.join();
}
