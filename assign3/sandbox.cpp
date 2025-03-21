#include "class.h"
#include <iostream>

// Definición de la función sandbox que reemplaza al main original de este archivo.
void sandbox() {
    // Instancia usando el constructor por defecto
    MascotaVirtual pet1;
    std::cout << "Mascota 1: " << pet1.getNombre() << "\n";
    std::cout << "Energia: " << pet1.getEnergia() << "\n";
    std::cout << "Hambre: " << pet1.getHambre() << "\n";
    std::cout << "Felicidad: " << pet1.getFelicidad() << "\n\n";

    // Simular acciones con la mascota
    std::cout << "Alimentando a " << pet1.getNombre() << "...\n";
    pet1.alimentar(30);
    std::cout << "Hambre: " << pet1.getHambre() << "\n";
    std::cout << "Energia: " << pet1.getEnergia() << "\n\n";

    std::cout << "Jugando con " << pet1.getNombre() << "...\n";
    pet1.jugar(10);
    std::cout << "Felicidad: " << pet1.getFelicidad() << "\n";
    std::cout << "Energia: " << pet1.getEnergia() << "\n";
    std::cout << "Hambre: " << pet1.getHambre() << "\n\n";

    std::cout << pet1.getNombre() << " está durmiendo...\n";
    pet1.dormir(5);
    std::cout << "Energia: " << pet1.getEnergia() << "\n";
    std::cout << "Hambre: " << pet1.getHambre() << "\n";
    std::cout << "Felicidad: " << pet1.getFelicidad() << "\n\n";

    // Instancia usando el constructor personalizado
    MascotaVirtual pet2("Firulais", 80, 20, 90);
    std::cout << "Mascota 2: " << pet2.getNombre() << "\n";
    std::cout << "Energia: " << pet2.getEnergia() << "\n";
    std::cout << "Hambre: " << pet2.getHambre() << "\n";
    std::cout << "Felicidad: " << pet2.getFelicidad() << "\n";
}
