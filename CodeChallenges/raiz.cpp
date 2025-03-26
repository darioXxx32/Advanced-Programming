#include <iostream>
#include <cmath>
#include <utility>
#include <optional> // Para std::optional

// Función para resolver la ecuación cuadrática
std::optional<std::pair<double, double>> solveQuadratic(double a, double b, double c) {
    if (a == 0) { 
        // Caso de ecuación lineal: bx + c = 0
        if (b == 0) {
            return std::nullopt; // No hay solución si también c ≠ 0
        }
        double x = -c / b;
        return {{x, x}}; // Misma solución dos veces para mantener el formato
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return std::nullopt; // No hay soluciones reales
    }

    double sqrt_discriminant = std::sqrt(discriminant);
    double x1 = (-b + sqrt_discriminant) / (2 * a);
    double x2 = (-b - sqrt_discriminant) / (2 * a);

    return {{x1, x2}};
}

int main() {
    double a, b, c;

    // Pedir los coeficientes por separado
    std::cout << "Ingrese el coeficiente a: ";
    std::cin >> a;

    std::cout << "Ingrese el coeficiente b: ";
    std::cin >> b;

    std::cout << "Ingrese el coeficiente c: ";
    std::cin >> c;

    auto result = solveQuadratic(a, b, c);
    
    if (result) {
        if (a == 0) {
            std::cout << "Ecuación lineal, solución única: x = " << result->first << std::endl;
        } else {
            std::cout << "Las soluciones son: x1 = " << result->first 
                      << ", x2 = " << result->second << std::endl;
        }
    } else {
        std::cout << "No hay soluciones reales." << std::endl;
    }

    return 0;
}
