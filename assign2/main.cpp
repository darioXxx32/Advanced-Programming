#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

std::string kYourName = "Dario Pomasqui"; // Cambia esto a tu nombre completo

/**
 * Lee un archivo y devuelve un conjunto de nombres de applicants.
 */
std::set<std::string> get_applicants(const std::string& filename) {
    std::set<std::string> applicants;
    std::ifstream file(filename);
    std::string name;
    
    if (!file) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return applicants;
    }
    
    while (getline(file, name)) {
        applicants.insert(name);
    }
    
    return applicants;
}

/**
 * Obtiene las iniciales de un nombre.
 */
std::string get_initials(const std::string& name) {
    if (name.empty()) return "";
    
    size_t space = name.find(' ');
    if (space == std::string::npos || space + 1 >= name.size()) return ""; // Validación adicional
    
    return std::string(1, name[0]) + std::string(1, name[space + 1]);
}

/**
 * Filtra los nombres que tienen las mismas iniciales y los guarda en una queue.
 */
std::queue<std::string> find_matches(const std::string& name, const std::set<std::string>& students) {
    std::queue<std::string> matches;
    std::string initials = get_initials(name);
    
    for (const auto& student : students) {
        if (get_initials(student) == initials) {
            matches.push(student);
        }
    }
    return matches;
}

/**
 * Selecciona el "one true match" de la lista de posibles matches.
 */
std::string get_match(std::queue<std::string>& matches) {
    if (matches.empty()) {
        return "NO MATCHES FOUND.";
    }
    
    // Aquí simplemente tomamos el primer elemento, pero se puede mejorar con aleatoriedad u otros criterios.
    std::string match = matches.front();
    return match;
}

// Eliminamos la definición de main() para evitar la redefinición.
// int main() {
//     std::set<std::string> applicants = get_applicants("students.txt");
//     
//     std::queue<std::string> matches = find_matches(kYourName, applicants);
//     
//     std::string match = get_match(matches);
//     std::cout << "Your Marriage Pact match is: " << match << std::endl;
//     
//     return 0;
// }

#include "autograder/utils.hpp"
