#ifndef CONEXIONBD_H
#define CONEXIONBD_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <iostream>

class ConexionBD {
public:
    ConexionBD();
    ~ConexionBD();
    void conectar();

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
};

#endif // CONEXIONBD_H
