#include "../src/fb_ejercicio.hpp"
#include <iostream>

int main() {
    // Instancia de referencia oficial: "abc12" sobre alfabeto A2, n=5
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789"; // A2
    std::string target_hash = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

    std::cout << "Buscando con fuerza bruta (puede tardar unos segundos)...\n";
    FBResult result = search_by_brute_force(target_hash, alphabet, 5);

    std::cout << "Encontrada: " << (result.found ? "SI" : "NO") << "\n";
    std::cout << "Password: " << result.password << "\n";
    std::cout << "Candidatos evaluados: " << result.candidates_evaluated << "\n";
    std::cout << "Tiempo (ms): " << result.execution_time_ms << "\n";

    return 0;
}
