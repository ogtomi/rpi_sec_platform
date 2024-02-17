#include "db_connect.hpp"

Connect_DB::Connect_DB(std::string con_credentials)
    : C(con_credentials)
{   
    check_connection(C);
}

void Connect_DB::check_connection(pqxx::connection &C)
{
    if(C.is_open())
    {
        std::cout << "Successfully connected to: " << C.dbname() << std::endl;        
    }
    else
    {
        std::cout << "Cannot connect to db" << std::endl;
    } 
}

void Connect_DB::create_table()
{
    const char *sql = "CREATE TABLE USR_CREDENTIALS(" \
        "NAME       TEXT        NOT NULL," \
        "PASSWORD   CHAR(32))";
    
    pqxx::work W(C);
    W.exec(sql);
    W.commit();

    std::cout << "Table created successfully" << std::endl;
}

int main(int argc, char* argv[])
{
    std::string con_credentials = "dbname = testdb user = postgres password = test1234 hostaddr = 127.0.0.1 port = 5432";
    
    Connect_DB connect_db(con_credentials);
    connect_db.create_table();

}