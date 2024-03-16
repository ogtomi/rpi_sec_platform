#include <iostream>
#include <pqxx/pqxx>

class Manage_DB
{
private:
    pqxx::connection C;

public:
    Manage_DB(std::string con_credentials);
    bool execute_transactional(const std::string &sql);
    void select(std::string sql);
    bool user_exists(const std::string &username);
    ~Manage_DB();

private:
    void check_connection(pqxx::connection &C);
};