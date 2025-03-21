#include "class.h"
#include <algorithm>  // Para std::max, std::min y std::clamp

// Función privada para actualizar el estado interno de la mascota
void MascotaVirtual::actualizarEstado() {
    // Ejemplo simple: si el hambre es muy alto o la energía muy baja, se reduce la felicidad.
    if (hambre > 80 || energia < 20) {
        felicidad = std::max(0, felicidad - 5);
    } else {
        felicidad = std::min(100, felicidad + 2);
    }
}

// Constructor por defecto
MascotaVirtual::MascotaVirtual() 
    : nombre("Mascota"), energia(100), hambre(0), felicidad(100) {
    actualizarEstado();
}

// Constructor personalizado
MascotaVirtual::MascotaVirtual(const std::string &nombre, int energia, int hambre, int felicidad)
    : nombre(nombre), energia(energia), hambre(hambre), felicidad(felicidad) {
    actualizarEstado();
}

// Getters
std::string MascotaVirtual::getNombre() const {
    return nombre;
}

int MascotaVirtual::getEnergia() const {
    return energia;
}

int MascotaVirtual::getHambre() const {
    return hambre;
}

int MascotaVirtual::getFelicidad() const {
    return felicidad;
}

// Setters
void MascotaVirtual::setNombre(const std::string &nuevoNombre) {
    nombre = nuevoNombre;
}

void MascotaVirtual::setEnergia(int nuevaEnergia) {
    energia = std::clamp(nuevaEnergia, 0, 100);
    actualizarEstado();
}

void MascotaVirtual::setHambre(int nuevaHambre) {
    hambre = std::clamp(nuevaHambre, 0, 100);
    actualizarEstado();
}

void MascotaVirtual::setFelicidad(int nuevaFelicidad) {
    felicidad = std::clamp(nuevaFelicidad, 0, 100);
    // No es necesario actualizar el estado en este setter
}

// Método para simular alimentar a la mascota
void MascotaVirtual::alimentar(int cantidad) {
    // Alimentar reduce el hambre y aumenta la energía (en proporción a la cantidad)
    hambre = std::max(0, hambre - cantidad);
    energia = std::min(100, energia + cantidad / 2);
    actualizarEstado();
}

// Método para simular jugar con la mascota
void MascotaVirtual::jugar(int duracion) {
    // Jugar aumenta la felicidad, reduce la energía y aumenta el hambre
    felicidad = std::min(100, felicidad + duracion);
    energia = std::max(0, energia - duracion * 2);
    hambre = std::min(100, hambre + duracion);
    actualizarEstado();
}

// Método para simular que la mascota duerme
void MascotaVirtual::dormir(int duracion) {
    // Dormir aumenta la energía pero también incrementa el hambre
    energia = std::min(100, energia + duracion * 3);
    hambre = std::min(100, hambre + duracion * 2);
    actualizarEstado();
}
