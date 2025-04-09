// utils.cpp
#include "utils.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void ejecutarConsulta(const string &consultaSQL) {
    string comando = "mysql -u root -pp1234 -D biblioteca -e \"" + consultaSQL + "\"";
    cout << "Ejecutando comando: " << comando << endl;
    int resultado = system(comando.c_str());
    if (resultado != 0) {
        cerr << "Error al ejecutar la consulta." << endl;
    }
}
