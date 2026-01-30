#pragma once
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class ConnexioBD {
private:
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    static ConnexioBD* instance; // Puntero estático para la instancia única

    // Constructor privado para evitar instanciación externa
    ConnexioBD();

    // Eliminar el constructor de copia y el operador de asignación
    ConnexioBD(const ConnexioBD& obj) = delete; // Eliminar constructor de copia
    ConnexioBD& operator=(const ConnexioBD& obj) = delete; // Eliminar operador de asignación

public:
    // Método estático para obtener la instancia única
    static ConnexioBD* getInstance();

    // Destructor
    ~ConnexioBD();

    // Método para ejecutar consultas SELECT
    sql::ResultSet* consulta(const string& sql);

    // Método para ejecutar comandos SQL (INSERT, UPDATE, DELETE)
    void executa(const string& sql);
};