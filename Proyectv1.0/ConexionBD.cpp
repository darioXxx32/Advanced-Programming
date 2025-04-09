#include "ConexionBD.h"

// ConexionBD.cpp
#include "utils.h"

#include <mysqlx/xdevapi.h>


using namespace std;

ConexionBD::ConexionBD() {
    driver = nullptr;
    con = nullptr;
}

ConexionBD::~ConexionBD() {
    if (con) {
        delete con;
    }
}

void ConexionBD::conectar() {
    try {
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "tu_contraseña"); // Cambia según tu configuración
        con->setSchema("proyecto");  // Asume que la base de datos se llama 'biblioteca'
        cout << "Conexión exitosa a la base de datos!" << endl;
    } catch (sql::SQLException &e) {
        cerr << "Error al conectar a la base de datos: " << e.what() << endl;
    }
}
