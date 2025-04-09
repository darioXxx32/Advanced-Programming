// Biblioteca.cpp
#include "Biblioteca.h"
#include <iostream>
#include <cstdlib>   // Para system()
#include "utils.h"   // Asegúrate de que incluir 'utils.h' esté aquí
using namespace std;

void generarInsertLibro(const string &titulo, const string &autor) {
    // Aquí armas tu consulta SQL
    string consultaSQL = "INSERT INTO libros (titulo, autor, disponible) VALUES ('" + titulo + "', '" + autor + "', 1);";
    cout << "SQL generado: " << consultaSQL << endl;

    // Ahora la ejecutas
    ejecutarConsulta(consultaSQL);
}

void agregarLibro() {
    string titulo, autor;
    cout << "Ingrese titulo: ";
    cin >> titulo;
    cout << "Ingrese autor: ";
    cin >> autor;

    // Llamas a la función que genera e inserta el libro
    generarInsertLibro(titulo, autor);
}
