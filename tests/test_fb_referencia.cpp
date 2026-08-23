#include "../src/fb_ejercicio.hpp"
#include <iostream>

int main() {
    int fallos = 0;

    // --- Test 1: Fuerza bruta contra instancia de referencia oficial ---
    // Contraseña "abc12" sobre alfabeto A2 (minúsculas + dígitos), n=5
    {
        std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
        std::string target_hash = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

        std::cout << "[Test 1] Fuerza bruta - instancia de referencia (abc12/A2/n=5)...\n";
        FBResult result = search_by_brute_force(target_hash, alphabet, 5);

        if (result.found && result.password == "abc12") {
            std::cout << "  PASO. Encontrada: " << result.password
                      << " | Candidatos: " << result.candidates_evaluated
                      << " | Tiempo: " << result.execution_time_ms << " ms\n";
        } else {
            std::cerr << "  FALLO. Se esperaba 'abc12', se obtuvo: '"
                      << result.password << "' (found=" << result.found << ")\n";
            fallos++;
        }
    }

    // --- Test 2: Diccionario oficial, probando la contraseña de referencia 'abc12' ---
    {
        std::string target_hash = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";
        std::string dict_path = "resources/diccionario.txt";

        std::cout << "[Test 2] Diccionario - palabra presente ('abc12')...\n";
        FBResult result = search_by_dictionary(target_hash, dict_path);

        if (result.found && result.password == "abc12") {
            std::cout << "  PASO. Encontrada: " << result.password
                      << " | Candidatos: " << result.candidates_evaluated
                      << " | Tiempo: " << result.execution_time_ms << " ms\n";
        } else {
            std::cerr << "  FALLO. No se encontro 'abc12' en el diccionario oficial.\n";
            fallos++;
        }
    }

    // --- Resumen final ---
    std::cout << "\n===============================\n";
    if (fallos == 0) {
        std::cout << "TODOS LOS TESTS PASARON.\n";
    } else {
        std::cout << fallos << " TEST(S) FALLARON.\n";
    }

    return fallos == 0 ? 0 : 1;
}
