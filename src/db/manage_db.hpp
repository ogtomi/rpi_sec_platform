#include <iostream>
#include <pqxx/pqxx>

class Manage_DB
{
private:
    pqxx::connection C;

public:
    Manage_DB(std::string con_credentials);
    void execute_transactional(std::string sql);
    void select(std::string sql);
    ~Manage_DB();

private:
    void check_connection(pqxx::connection &C);
};