#include <iostream>
#include <pqxx/pqxx>

class Manage_DB
{
private:
    pqxx::connection C;

public:
    Manage_DB(std::string con_credentials);
    bool execute_transactional(std::string sql);
    void select(std::string sql);
    bool user_exists(char* username);
    ~Manage_DB();

private:
    void check_connection(pqxx::connection &C);
};