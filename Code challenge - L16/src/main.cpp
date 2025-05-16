#include "CLI11.hpp"
#include "barkeep/barkeep.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace bk = barkeep;

// Función para separar una línea CSV respetando comillas
static std::vector<std::string> splitCSV(const std::string &line) {
    std::vector<std::string> fields;
    std::string cur;
    bool in_quotes = false;
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            fields.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    fields.push_back(cur);
    return fields;
}

int main(int argc, char** argv) {
    // 1) CLI11
    CLI::App app{"Retail Data Analytics"};
    std::string country_filter;
    bool only_uk = false;
    app.add_option("-c,--country", country_filter, "Filtrar por país");
    app.add_flag("--only-uk", only_uk, "Sólo calcular UK");
    CLI11_PARSE(app, argc, argv);

    // 2) Abre CSV y cuenta líneas (sin contar cabecera)
    std::ifstream file("data/OnlineRetail.csv");
    if (!file) {
        std::cerr << "No puedo abrir data/OnlineRetail.csv\n";
        return 1;
    }
    std::string linea;
    // Lee cabecera
    if (!std::getline(file, linea)) {
        std::cerr << "CSV vacío o sin cabecera\n";
        return 1;
    }

    int total_lines = 0;
    std::streampos pos = file.tellg();
    while (std::getline(file, linea)) {
        ++total_lines;
    }
    // Reinicia al principio de datos
    file.clear();
    file.seekg(pos);

    // 3) Configura barra y contadores
    int work = 0;
    auto bar = bk::ProgressBar(&work, {
      .total = total_lines,
      .message = "Procesando"
    });
    std::map<std::string,long> counts;
    double total_amount = 0.0;

    // 4) Procesa cada línea usando splitCSV
    while (std::getline(file, linea)) {
        ++work;
        auto cols = splitCSV(linea);
        if (cols.size() < 8) continue;  // línea mal
        std::string quantity_s = cols[3];
        std::string price_s    = cols[5];
        std::string country    = cols[7];

        // Filtrar
        if (!country_filter.empty() && country != country_filter) continue;
        if (only_uk && country != "United Kingdom") continue;

        // Convertir y acumular
        try {
            long qty = std::stol(quantity_s);
            double pr = std::stod(price_s);
            counts[country]++;
            total_amount += qty * pr;
        } catch (...) {
            continue;
        }
    }
    bar->done();
    file.close();

    // 5) Imprime resultados
    std::cout << "\n-- Transacciones --\n";
    if (only_uk || !country_filter.empty()) {
        std::string key = only_uk ? "United Kingdom" : country_filter;
        std::cout << key << ": " << counts[key] << "\n";
    } else {
        for (auto& [k,v] : counts)
            std::cout << k << ": " << v << "\n";
    }

    std::cout << "\n-- Monto total --\n";
    if (only_uk) {
        std::cout << "United Kingdom: " << total_amount << "\n";
    } else if (!country_filter.empty()) {
        std::cout << country_filter << ": " << total_amount << "\n";
    } else {
        std::cout << "Todos los países: " << total_amount << "\n";
    }

    return 0;
}
