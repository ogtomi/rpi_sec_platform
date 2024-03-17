#include "user_interface.hpp"

void UserInterface::get_menu(BaseMessage &write_msg)
{
    const char* menu = "\
        \n-USER MENU-\n\
        1. Create user\n\
        2. Login\n\
    ";

    write_msg.body_length(std::strlen(menu));
    std::memcpy(write_msg.body(), menu, write_msg.body_length());
}

int UserInterface::split_response(char* read_msg_body, std::vector<std::string> &output_buff)
{   
    const char* delimiter = " ";
    int i = 0;

    char* token = std::strtok(read_msg_body, delimiter);

    if(token != NULL)
    {
        output_buff.push_back(token);
    }

    while(token != NULL)
    {
        i++;
        token = std::strtok(NULL, delimiter);

        if(token != NULL)
        {
            output_buff.push_back(token);
        }
    }

    return i;
}

void UserInterface::read_response(char* read_msg_body, BaseMessage &write_msg)
{
    int argc;
    std::string msg;
    std::vector<std::string> split_msg;

    argc = split_response(read_msg_body, split_msg);

    switch(std::stoi(split_msg[0]))
    {
        case 1:
            if(argc != 3)
            {
                msg = "Invalid number of parameters.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
                break;  
            }
            if(dbmg.user_exists(split_msg[1].c_str()))
            {
                msg = "User already exists.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
                break;
            }
            if(!create_user(split_msg[1], split_msg[2], split_msg[2].size()))
            {
                msg = "Error while creating an user.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            }
            else
            {
                msg = "Successfully created user.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            }
            break;
        
        case 2:
            if(argc != 3)
            {
                msg = "Invalid number of parameters.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
                break;  
            }
            if(!dbmg.user_exists(split_msg[1].c_str()))
            {
                msg = "User does not exist.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
                break;
            }
            if(login(split_msg[1], split_msg[2], split_msg[2].size()))
            {
                msg = "Successfully logged in.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            }
            else
            {
                msg = "Wrong credentials.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            }
            break;
        
        default:
            msg = "Invalid option.";
            write_msg.body_length(msg.size());
            std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            break;
    }
}

bool UserInterface::create_user(const std::string &username, const std::string &password, const size_t password_size)
{
    unsigned char hash_password[SHA256_DIGEST_LENGTH];
    char hash_buff[BaseMessage::hash_length + 1];
    char *ptr = &hash_buff[0];

    crypto.sha256(password.c_str(), password_size, hash_password);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ptr += std::sprintf(ptr, "%02x", hash_password[i]);
    }

    std::string hash_password_str(hash_buff);

    return dbmg.add_user(username, hash_password_str);
}

bool UserInterface::login(const std::string &username, const std::string &password, const std::size_t password_size)
{
    std::string hash_password_db = dbmg.get_password_hash(username, password);
    unsigned char hash_password[SHA256_DIGEST_LENGTH];
    char hash_buff[BaseMessage::hash_length + 1];
    char *ptr = &hash_buff[0];

    crypto.sha256(password.c_str(), password_size, hash_password);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ptr += std::sprintf(ptr, "%02x", hash_password[i]);
    }

    std::string hash_password_str(hash_buff);

    if(hash_password_db == hash_password_str)
        return true;

    return false;
}
