#include "manage_db.hpp"

Manage_DB::Manage_DB(std::string con_credentials)
    : C(con_credentials)
{   
    check_connection(C);
}

Manage_DB::~Manage_DB()
{
    C.disconnect();
}

void Manage_DB::check_connection(pqxx::connection &C)
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

void Manage_DB::execute_transactional(std::string sql)
{
    try
    {
        pqxx::work W(C);
        W.exec(sql);
        W.commit();

        std::cout << "Operation executed successfully" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Manage_DB::select(std::string sql)
{
    try
    {
        pqxx::nontransaction N(C);
        pqxx::result R(N.exec(sql));

        for(pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c)
        {
            std::cout << "User: " << c[0].as<std::string>() << std::endl;
        }

        std::cout << "Operation done successfully" << std::endl;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
