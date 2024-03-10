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

void UserInterface::split_response(char* read_msg_body, char** output_buff)
{   
    const char* delimiter = " ";
    int i = 0;

    output_buff[i] = std::strtok(read_msg_body, delimiter);

    while(output_buff[i] != NULL && i < 2)
    {
        i++;
        output_buff[i] = std::strtok(NULL, delimiter);
    }
}

void UserInterface::read_response(char* read_msg_body, BaseMessage &write_msg)
{
    std::string msg;
    char* split_msg[3];

    split_response(read_msg_body, split_msg);

    switch(std::atoi(split_msg[0]))
    {
        case 1:
            if(dbmg.user_exists(split_msg[1]))
            {
                msg = "User already exists.";
                write_msg.body_length(msg.size());
                std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
                break;
            }
            if(!create_user(split_msg[1], split_msg[2], std::strlen(split_msg[2])))
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
            msg = "Successfully completed option 2.";
            write_msg.body_length(msg.size());
            std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            break;
        
        default:
            msg = "Invalid option.";
            write_msg.body_length(msg.size());
            std::memcpy(write_msg.body(), msg.c_str(), write_msg.body_length());
            break;
    }
}

bool UserInterface::create_user(char* username, char* password, size_t password_size)
{
    unsigned char hash_password[SHA256_DIGEST_LENGTH];
    char hash_buff[BaseMessage::hash_length + 1];
    char *ptr = &hash_buff[0];

    crypto.sha256(password, password_size, hash_password);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ptr += std::sprintf(ptr, "%02x", hash_password[i]);
    }

    std::string username_str(username);
    std::string hash_password_str(hash_buff);

    std::string sql_cmd = "INSERT INTO usr_credentials (name, password) " \
        "VALUES ('" + username_str + "' , '" + hash_password_str + "' );";
        
    return dbmg.execute_transactional(sql_cmd);
}
