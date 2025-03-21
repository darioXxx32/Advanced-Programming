#ifndef MASCOTAVIRTUAL_H
#define MASCOTAVIRTUAL_H

#include <string>

class MascotaVirtual {
private:
    std::string nombre;
    int energia;
    int hambre;
    int felicidad;

    // Función privada para actualizar el estado interno de la mascota
    void actualizarEstado();

public:
    // Constructor por defecto
    MascotaVirtual();

    // Constructor personalizado con parámetros
    MascotaVirtual(const std::string &nombre, int energia, int hambre, int felicidad);

    // Getters (marcados como const)
    std::string getNombre() const;
    int getEnergia() const;
    int getHambre() const;
    int getFelicidad() const;

    // Setters
    void setNombre(const std::string &nuevoNombre);
    void setEnergia(int nuevaEnergia);
    void setHambre(int nuevaHambre);
    void setFelicidad(int nuevaFelicidad);

    // Métodos públicos para simular acciones de la mascota
    void alimentar(int cantidad);  // Disminuye el hambre y aumenta algo de energía
    void jugar(int duracion);       // Aumenta la felicidad, pero gasta energía y aumenta el hambre
    void dormir(int duracion);      // Recupera energía, aunque incrementa el hambre
};

#endif // MASCOTAVIRTUAL_H
