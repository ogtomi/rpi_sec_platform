#include <iostream>
#include <pqxx/pqxx>

class Connect_DB
{
private:
    pqxx::connection C;

public:
    Connect_DB(std::string con_credentials);
    void create_table();

private:
    void check_connection(pqxx::connection &C);
};