#include <iostream>
#include <fstream>
#include <string>
#include "Biblioteca.h"  // Aquí irán las funciones de lógica de negocio
#include "utils.h"       // Donde declaras ejecutarConsulta(...)

using namespace std;

int main() {
    int opcion;
    do {
        cout << "\n=== Sistema de Biblioteca ===\n";
        cout << "1. Agregar libro\n";
        cout << "2. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();  // Limpiar el buffer de entrada

        switch(opcion) {
            case 1: {
                string titulo, autor;
                cout << "Ingrese título: ";
                getline(cin, titulo);
                cout << "Ingrese autor: ";
                getline(cin, autor);

                // Llamas a la función que genera y ejecuta el comando SQL
                generarInsertLibro(titulo, autor);  

                break;
            }
            case 2:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opción inválida\n";
        }
    } while(opcion != 2);

    return 0;
}
