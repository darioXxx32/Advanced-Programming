#include <iostream>
#include <string>
#include <concepts>  
using namespace std;
/*Dario Pomasqui/

/* -------------------------------------------------------------------------
 * PART 1: BASIC TEMPLATE FUNCTIONS
 * ------------------------------------------------------------------------- */

//  Task 1.1: Write a Simple Template Function
template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}

//  Task 1.2: Overload a Template Function (Tres parámetros)
template <typename T>
T maxValue(T a, T b, T c) {
    return maxValue(a, maxValue(b, c));
}

/* -------------------------------------------------------------------------
 * PART 2: CONCEPTS
 * ------------------------------------------------------------------------- */

//  Task 2.1: Create a Template with a Concept
// 1) Crear un concepto "Numeric" que acepte tipos integrales o de punto flotante.
template <typename T>
concept Numeric = integral<T> || floating_point<T>;

// 2) Escribir una función plantilla "add" que acepte dos parámetros restringidos por el concepto Numeric
template <Numeric T>
T add(T a, T b) {
    return a + b;
}

/* -------------------------------------------------------------------------
 * PART 3: VARIADIC TEMPLATES
 * ------------------------------------------------------------------------- */

//  Task 3.1: Write a Variadic Template Function (printAll)
template <typename T>
void printAll(T t) {
    std::cout << t << std::endl;
}

template <typename T, typename... Args>
void printAll(T t, Args... args) {
    std::cout << t << " ";
    printAll(args...);
}

//  Task 3.2: Variadic Template for Summation
// Caso base: un solo elemento
template <typename T>
T sum(T t) {
    return t;
}

// Caso recursivo: suma el primer elemento con el resultado del resto
template <typename T, typename... Args>
T sum(T t, Args... args) {
    return t + sum(args...);
}

/* -------------------------------------------------------------------------
 * PART 4: TEMPLATE METAPROGRAMMING
 * ------------------------------------------------------------------------- */

//  Task 4.1: Compile-Time Factorial Using Templates
template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

// Especialización para el caso base (N = 0)
template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

//  Task 4.2: Fibonacci Series Using Template Metaprogramming
template <int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

// Casos base para Fibonacci
template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

/* -------------------------------------------------------------------------
 * MAIN: Ejemplos de prueba
 * ------------------------------------------------------------------------- */

int main() {
    // Cálculo en tiempo de compilación (Fibonacci)
    constexpr int fib10 = Fibonacci<10>::value;   // Fibonacci(10) = 55

    // Cálculo en tiempo de compilación (Factorial)
    constexpr int fact5 = Factorial<5>::value;    // 5! = 120

    // PART 1: Pruebas de maxValue
    cout << "Máximo entre 5 y 10: " 
         << maxValue(5, 10) << endl;                     // 10

    cout << "Máximo entre 3.5 y 2.1: " 
         << maxValue(3.5, 2.1) << endl;                  // 3.5

    cout << "Máximo entre \"apple\" y \"orange\": " 
         << maxValue(string("apple"), string("orange"))  // "orange"
         << endl;

    // Sobrecarga de 3 parámetros
    cout << "Máximo entre 5, 10 y 15: " 
         << maxValue(5, 10, 15) << endl;                 // 15

    cout << "Máximo entre 5.3, 5.6 y 6.2: " 
         << maxValue(5.3, 5.6, 6.2) << endl;             // 6.2

    // PART 2: Prueba de la función add con el concepto Numeric
    cout << "Suma de 3 + 4: " << add(3, 4) << endl;       // 7
    cout << "Suma de 2.5 + 3.1: " << add(2.5, 3.1) << endl; // 5.6
    // cout << add("a", "b");  // Descomentar para ver que falla en compilación (no es Numeric)

    // PART 3: Prueba de printAll y sum
    printAll(1, 2.5, 32, "Hello", 'c'); 
    // Salida esperada: 1 2.5 32 Hello c

    printAll("Hola mundo");
    

    cout << "Suma de 1, 2, 3, 4, 5: " 
         << sum(1, 2, 3, 4, 5) << endl;                   

    // PART 4: Pruebas de Template Metaprogramming
    cout << "Factorial de 5: " << fact5 << endl;          
    cout << "Fibonacci(10): " << fib10 << endl;           // 55

    return 0;
}
