#include <iostream>
#include <pqxx/pqxx>

class Manage_DB
{
private:
    pqxx::connection C;

public:
    Manage_DB(std::string con_credentials);
    bool add_user(const std::string &username, const std::string &hash_password);
    std::string get_password_hash(const std::string &username, const std::string &hash_password);
    bool user_exists(const std::string &username);
    ~Manage_DB();

private:
    void check_connection(pqxx::connection &C);
};