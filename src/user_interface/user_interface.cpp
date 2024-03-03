#include "user_interface.hpp"

void UserInterface::get_menu(BaseMessage &write_msg)
{
    const char* menu = "\
        \n-USER MENU-\n\
        1. Create user\n\
        2. Login\n\
        3. Show content\n\
    ";

    write_msg.body_length(std::strlen(menu));
    std::memcpy(write_msg.body(), menu, write_msg.body_length());
}

void UserInterface::read_response(char* read_msg_body, BaseMessage &write_msg)
{
    char* msg;
    switch(std::atoi(read_msg_body))
    {
        case 1:
            msg = "Successfully completed option 1.";
            write_msg.body_length(std::strlen(msg));
            std::memcpy(write_msg.body(), msg, write_msg.body_length());
            break;
        
        case 2:
            msg = "Successfully completed option 2.";
            write_msg.body_length(std::strlen(msg));
            std::memcpy(write_msg.body(), msg, write_msg.body_length());
            break;
        
        case 3:
            msg = "Successfully completed option 3.";
            write_msg.body_length(std::strlen(msg));
            std::memcpy(write_msg.body(), msg, write_msg.body_length());
            break;
        
        default:
            msg = "Invalid option.";
            write_msg.body_length(std::strlen(msg));
            std::memcpy(write_msg.body(), msg, write_msg.body_length());
            break;
    }
}