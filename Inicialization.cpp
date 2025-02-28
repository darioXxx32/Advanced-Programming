#include <iostream>
#include <string>
#include <tuple>
#include <cstdio>

// Parte 4: Definición de estructura
struct Point {
    int x, y, z;
};

int main(void) {
    // 🟢 Parte 1: Inicialización de variables con diferentes métodos
    int a = 10;                // Inicialización directa
    double b{3.14};            // Inicialización uniforme (C++11)
    std::string c("Hello");    // Inicialización con constructor
    bool d = true;             // Inicialización con asignaciónp
    
  
    // Mostrar valores iniciales
    std::cout << "Valores iniciales:\n";
    std::cout << "int: " << a << ", double: " << b << ", string: " << c << ", bool: " << d << "\n\n";

    // 🟢 Parte 3: Referencias y modificación de valores
    int& refA = a;       // Referencia a 'a'
    double& refB = b;    // Referencia a 'b'
    std::string& refC = c;  // Referencia a 'c'
    bool& refD = d;      // Referencia a 'd'

    // Modificamos usando referencias
    refA = 20;
    refB = 2.71;
    refC = "World";
    refD = false;
    refA=30;

    // Mostrar valores modificados
    std::cout << "Valores modificados:\n";
    std::cout << "int: " << a << ", double: " << b << ", string: " << c << ", bool: " << d << "\n\n";

    // 🟢 Parte 3: Uso de punteros para modificar valores
    int* ptrA = &a; // Puntero a 'a'
    *ptrA = 100;    // Modificar usando el puntero
    //podemos continuar con el resto de valores

    // Mostrar valor modificado mediante puntero
    std::cout << "Modificación por puntero:\n";
    std::cout << "int: " << a << "\n\n";

    // 🟢 Parte 4: Structured Binding con un struct
    Point p{5, 10, 15}; // Inicialización uniforme
    auto [x, y, z] = p; // Desestructuración

    // Mostrar valores desestructurados
    std::cout << "Binding estructurado:\n";
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n\n";

    // 🟢 Parte 5: Identificación de l-values y r-values
    a = 42;             // 'a' es un l-value (tiene un nombre y puede ser modificado)
    int x1 = a + b;     // 'a' y 'b' son l-values, pero 'a + b' es un r-value (es una expresión temporal)
    &a;                 // '&a' es un l-value (puede obtener su dirección)
    std::string s = c + " World"; // 'c' es un l-value, " World" es un r-value, y (c + " World") es un r-value
    int& refX = x1;     // 'refX' es una referencia a un l-value
    int&& rref = 100;   // '100' es un r-value, y 'rref' es una referencia a un r-value (r-value reference)

    return 0;
}
