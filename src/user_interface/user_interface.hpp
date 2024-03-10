#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include <iostream>
#include "../messages/base_message.hpp"
#include "../db/manage_db.hpp"

class UserInterface
{
private:
    Manage_DB dbmg{"dbname = testdb user = postgres password = test1234 hostaddr = 127.0.0.1 port = 5432"};
    Crypto crypto;

    bool create_user(char* username, char* password, size_t password_size);
    int split_response(char* read_msg_body, char** output_buff);

public:
    void get_menu(BaseMessage &write_msg);
    void read_response(char* read_msg_body, BaseMessage &write_msg);
};

#endif