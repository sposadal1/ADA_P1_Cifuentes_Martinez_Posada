#include "../src/fb_ejercicio.hpp"
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::cout << "=== Test de Verificación de Semilla e Instancias del Equipo ===\n\n";

    // 1. Probar cálculo de la semilla
    std::string apellidos = "cifuentesmartinezposada";
    long long semilla = calcular_semilla(apellidos);

    std::cout << "Apellidos evaluados: " << apellidos << "\n";
    std::cout << "Semilla calculada: " << semilla << "\n";

    // Verificación de rango válido (0 a 99999)
    if (semilla >= 0 && semilla < 100000) {
        std::cout << "  [PASO] La semilla está dentro del rango esperado [0, 99999].\n\n";
    } else {
        std::cerr << "  [FALLO] La semilla está fuera de rango.\n\n";
        return 1;
    }

    // 2. Probar generación de las 5 contraseñas sintéticas
    std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::vector<std::string> alfabetos = {A1, A2, A1, A2, A1};
    int longitudes_esperadas[5] = {4, 4, 5, 5, 6};

    std::vector<std::string> passwords = generar_passwords_equipo(semilla, alfabetos);

    if (passwords.size() != 5) {
        std::cerr << "  [FALLO] Se esperaban 5 contraseñas generadas.\n";
        return 1;
    }

    std::cout << "Contraseñas generadas por el LCG:\n";
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        std::cout << "  Instancia " << (i + 1) << ": '" << passwords[i] 
                  << "' (Longitud real: " << passwords[i].length() 
                  << ", Esperada: " << longitudes_esperadas[i] << ")\n";
        
        if (static_cast<int>(passwords[i].length()) != longitudes_esperadas[i]) {
            ok = false;
        }
    }

    std::cout << "\n===============================\n";
    if (ok) {
        std::cout << "TODOS LOS TESTS DE SEMILLA PASARON EXITOSAMENTE.\n";
        return 0;
    } else {
        std::cout << "HUBO ERRORES EN LAS LONGITUDES DE LAS CONTRASEÑAS.\n";
        return 1;
    }
}
