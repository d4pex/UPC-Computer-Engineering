#include "ConnexioBD.h"

// Inicialización del puntero estático a nullptr
ConnexioBD* ConnexioBD::instance = nullptr;

// Definición del constructor privado
ConnexioBD::ConnexioBD() {
    driver = sql::mysql::get_mysql_driver_instance();

    // Leer la configuración desde un archivo
    ifstream config("config.txt");
    string url, user, password, database;
    if (config.is_open()) {
        getline(config, url);
        getline(config, user);
        getline(config, password);
        getline(config, database);
        config.close();
    }
    else {
        throw runtime_error("No se pudo abrir el archivo de configuración");
    }

    // Establecer la conexión con la base de datos
    con = driver->connect(url, user, password);
    con->setSchema(database);
}

// Definición del destructor
ConnexioBD::~ConnexioBD() {
    if (con != nullptr) {
        con->close();
        delete con;
    }
}

// Definición del método estático para obtener la instancia
ConnexioBD* ConnexioBD::getInstance() {
    if (instance == nullptr) {
        instance = new ConnexioBD();
    }
    return instance;
}

// Definición del método para realizar consultas SELECT
sql::ResultSet* ConnexioBD::consulta(const string& sql) {
    sql::Statement* stmt = con->createStatement();
    return stmt->executeQuery(sql);
}

// Definición del método para ejecutar comandos SQL (INSERT, UPDATE, DELETE)
void ConnexioBD::executa(const string& sql) {
    sql::Statement* stmt = con->createStatement();
    stmt->execute(sql);
    delete stmt;
}