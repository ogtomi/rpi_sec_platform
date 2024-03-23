#include "client.hpp"

Client::Client(int domain, int service, int protocol, int port, u_long interface, struct sockaddr_in server)
{
    socket = new CSocket(domain, service, protocol, port, interface, server);

    if(!do_handshake())
    {
        perror("Handshake fail.");
        close(socket->get_sock());
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Connected to server." << std::endl;
    }
}

Client::~Client()
{
    delete socket;
}

CSocket* Client::get_socket()
{
    return socket;
}

bool Client::do_handshake()
{   
    // Client & Server keys (+ key messages)
    EVP_PKEY *ckey = NULL, *skey = NULL;
    Message server_pubkey_msg, client_pubkey_msg;

    // Key serialization vars
    size_t serialized_key_len = 0;
    unsigned char* serialized_key = NULL;
    
    // Shared secret (ECDH)
    unsigned char* secret = NULL;
    size_t secret_len = 0;
    unsigned char hashed_secret[SHA256_DIGEST_LENGTH] = {'\0'};
    
    // HANDSHAKE
    // Generate and serialize key
    ckey = crypto.generate_ec_key();
    if(ckey == NULL)
        return false;

    serialized_key = crypto.serialize_key(ckey, serialized_key_len);
    if(serialized_key == NULL)
        return false;

    // Read key from the server
    read(socket->get_sock(), server_pubkey_msg.data(), Message::header_length);

    if(!server_pubkey_msg.decode_header())
        return false;
    
    read(socket->get_sock(), server_pubkey_msg.body(), server_pubkey_msg.body_length());

    // Send the key to the server
    client_pubkey_msg.body_length(serialized_key_len);
    std::memcpy(client_pubkey_msg.body(), serialized_key, client_pubkey_msg.body_length());
    client_pubkey_msg.encode_header();
    write(socket->get_sock(), client_pubkey_msg.data(), client_pubkey_msg.length());

    // Obtain a shared secret using ECDH
    skey = crypto.deserialize_key((unsigned char*)server_pubkey_msg.body(), server_pubkey_msg.body_length());
    if(skey == NULL)
        return false;

    secret = crypto.ecdh(&secret_len, ckey, skey);
    if(secret == NULL)
        return false;
    
    // Hash the shared secret with SHA256
    crypto.sha256((char*)secret, secret_len, hashed_secret);

    // Use the first 128 bits as AES-128 key and the second 128 bits as IV
    std::memcpy(aes_128_key, hashed_secret, AES_128_KEY_SIZE);
    std::memcpy(iv, hashed_secret + AES_128_KEY_SIZE, IV_SIZE);

    free(serialized_key);
    free(secret);

    return true;
}

void Client::do_read()
{
    read(socket->get_sock(), read_msg.data(), BaseMessage::hash_length);
    read(socket->get_sock(), read_msg.header(), BaseMessage::header_length);

    if(!read_msg.decode_header())
        return;
    
    read(socket->get_sock(), read_msg.body(), read_msg.body_length());

    if(!read_msg.check_hash())
        return;

    read_msg.aes_128_cbc_decrypt(aes_128_key, iv);
    std::cout << "S: " << read_msg.body() << std::endl;
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

int main(int argc, char *argv[])
{
    struct hostent *hp;
    struct sockaddr_in server;

    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl;
        return 1;
    }

    hp = gethostbyname(argv[1]);
    std::memcpy(&server.sin_addr, hp->h_addr_list[0], hp->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    Client client(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, server);
    
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
