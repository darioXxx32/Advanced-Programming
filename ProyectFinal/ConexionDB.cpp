#define NOMINMAX

#include "ConexionDB.h"
#include <iostream>

// Constructor / destructor
ConexionBD::ConexionBD() : con(nullptr) {}
ConexionBD::~ConexionBD() { cerrar_conexion(); }

bool ConexionBD::abrir_conexion() {
    con = mysql_init(nullptr);
    if (!con) {
        std::cerr << "mysql_init failed\n";
        return false;
    }
    if (!mysql_real_connect(con,
        "localhost", "root", "p1234",
        "biblioteca", 3306, nullptr, 0))
    {
        std::cerr << "mysql_real_connect failed: "
            << mysql_error(con) << "\n";
        return false;
    }
    return true;
}

void ConexionBD::cerrar_conexion() {
    if (con) {
        mysql_close(con);
        con = nullptr;
    }
}

bool ConexionBD::login(const std::string& username,
    const std::string& password,
    std::string& outUserCedula,
    std::string& outRole)
{
    if (!con) return false;

    // 1) Escapar inputs
    std::string escUser(username.size(), '\0');
    std::string escPass(password.size(), '\0');

    unsigned long lenU = mysql_real_escape_string(
        con,
        &escUser[0],
        username.c_str(), static_cast<unsigned long>(username.length())
    );
    escUser.resize(static_cast<std::size_t>(lenU));

    unsigned long lenP = mysql_real_escape_string(
        con,
        &escPass[0],
        password.c_str(), static_cast<unsigned long>(password.length())
    );
    escPass.resize(static_cast<std::size_t>(lenP));

    // 2) Consulta: obtenemos role y cedula (id)
    std::string q =
        "SELECT role, id FROM users "
        "WHERE username = '" + escUser +
        "' AND password_hash = SHA2('" + escPass + "',256);";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error mysql_query: " << mysql_error(con) << "\n";
        return false;
    }

    MYSQL_RES* res = mysql_store_result(con);
    if (!res) return false;

    bool ok = false;
    if (MYSQL_ROW row = mysql_fetch_row(res)) {
        ok = true;
        outRole = row[0] ? row[0] : "";
        outUserCedula = row[1] ? row[1] : "";
    }

    mysql_free_result(res);
    return ok;
}
