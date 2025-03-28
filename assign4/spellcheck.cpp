#include "spellcheck.h"             // Incluye la declaración de funciones y tipos usados en spellcheck.
#include <algorithm>                // Para funciones como std::transform y std::erase_if.
#include <iostream>                 // Para operaciones de entrada/salida.
#include <numeric>                  // Para operaciones numéricas (si fueran necesarias).
#include <ranges>                   // Para usar los views (pipeline) de ranges.
#include <set>                      // Para usar std::set, que almacena elementos ordenados sin duplicados.
#include <vector>                   // Para usar std::vector.
#include <cctype>                   // Para la función isspace (determinar si un caracter es espacio).
#include "utils.cpp"                // Incluye las implementaciones auxiliares (como find_all y levenshtein).

// Declaración de la función auxiliar find_all (definida en utils.cpp).
template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

// ===========================================================
// SECTION 1: Implementación de tokenize
// ===========================================================
Corpus tokenize(const std::string& source) {
  // 1. Encuentra todos los iteradores en 'source' que apuntan a un caracter de espacio.
  auto space_iters = find_all(source.begin(), source.end(), [](char c) {
    return ::isspace(c);
  });

  // 2. Genera tokens usando cada par consecutivo de iteradores de espacios y los inserta en 'tokens'.
  Corpus tokens;
  std::transform(space_iters.begin(), space_iters.end() - 1,
                 space_iters.begin() + 1,
                 std::inserter(tokens, tokens.end()),
                 [&](auto it1, auto it2) {
                    // El constructor de Token extrae el substring entre it1 e it2, procesando espacios y puntuación.
                    return Token(source, it1, it2);
                 });

  // 3. Elimina los tokens vacíos, que pueden generarse por múltiples espacios consecutivos.
  std::erase_if(tokens, [](const Token &token) {
    return token.content.empty();
  });

  // Devuelve el conjunto de tokens obtenidos.
  return tokens;
}

// ===========================================================
// SECTION 2: Implementación de spellcheck
// ===========================================================
std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  // Alias para simplificar el uso de los views de ranges.
  namespace rv = std::ranges::views;

  // Filtra los tokens que no están en el diccionario (es decir, están mal escritos).
  auto misspelled_tokens = source | rv::filter([&](const Token& token) {
    return dictionary.find(token.content) == dictionary.end();
  });

  // Para cada token mal escrito, genera un objeto Mispelling con sugerencias:
  // Se buscan palabras en el diccionario cuya distancia Damerau-Levenshtein sea 1.
  auto mispelling_view = misspelled_tokens | rv::transform([&](const Token& token) {
    auto suggestions_view = dictionary | rv::filter([&](const std::string& word) {
      return levenshtein(token.content, word) == 1;
    });
    // Materializa las sugerencias en un set para ordenarlas y eliminar duplicados.
    std::set<std::string> suggestions(suggestions_view.begin(), suggestions_view.end());
    return Mispelling{ token, suggestions };
  });

  // Filtra los resultados para eliminar aquellos Mispelling que no tienen ninguna sugerencia.
  auto filtered_misspellings = mispelling_view | rv::filter([](const Mispelling& mispelling) {
    return !mispelling.suggestions.empty();
  });

  // Materializa la vista final en un std::set y lo retorna.
  std::set<Mispelling> result(filtered_misspellings.begin(), filtered_misspellings.end());
  return result;
}

// ===========================================================
// SECTION 3: Inclusión de métodos auxiliares
// ===========================================================
// Se incluye "utils.cpp" para proporcionar las implementaciones de funciones auxiliares,
// como find_all y levenshtein.