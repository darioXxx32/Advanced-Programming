#pragma once
#define NOMINMAX

#include <string>
#include <mysql.h>

class ConexionBD {
private:
    MYSQL* con;

public:
    ConexionBD();
    ~ConexionBD();

    bool abrir_conexion();
    void cerrar_conexion();

    /**
     * Intenta hacer login:
     * @param username          Nombre de usuario.
     * @param password          Contraseña en texto claro.
     * @param outUserCedula     (salida) Cédula del usuario si el login OK.
     * @param outRole           (salida) Rol del usuario si el login OK.
     * @return true si credenciales válidas, false en caso contrario.
     */
    bool login(const std::string& username,
        const std::string& password,
        std::string& outUserCedula,
        std::string& outRole);

    /** Devuelve el puntero crudo al conector para ejecutar consultas SQL. */
    MYSQL* getConector() const { return con; }
};
