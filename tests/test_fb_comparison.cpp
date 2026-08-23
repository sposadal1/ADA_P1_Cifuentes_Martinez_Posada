#include "../src/fb_ejercicio.hpp"
#include <iostream>

int main() {
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string target_hash = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";
    std::string dict_path = "resources/diccionario.txt";

    auto comp = compare_brute_force_vs_dictionary(target_hash, alphabet, 5, dict_path);

    std::cout << "Fuerza bruta -> Encontrada: " << (comp.brute_force_result.found ? "SI" : "NO")
               << " | Password: " << comp.brute_force_result.password
               << " | Tiempo: " << comp.brute_force_result.execution_time_ms << " ms"
               << " | Candidatos: " << comp.brute_force_result.candidates_evaluated << "\n";

    std::cout << "Diccionario  -> Encontrada: " << (comp.dictionary_result.found ? "SI" : "NO")
               << " | Password: " << comp.dictionary_result.password
               << " | Tiempo: " << comp.dictionary_result.execution_time_ms << " ms"
               << " | Candidatos: " << comp.dictionary_result.candidates_evaluated << "\n";

    return 0;
}