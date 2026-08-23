#include "third_party/picosha2.h"
#include "fb_ejercicio.hpp"
#include <iostream>
#include <string>

void run_fb_module() {
    std::cout << "=== Modulo Fuerza Bruta ===\n\n";

    // Instancia de referencia oficial 
    std::string alphabet_ref = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string hash_ref = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

    std::cout << "[Referencia] Buscando password para hash conocido (abc12/A2/n=5)...\n";
    FBResult ref_result = search_by_brute_force(hash_ref, alphabet_ref, 5);

    if (ref_result.found && ref_result.password == "abc12") {
        std::cout << "  OK: encontrada '" << ref_result.password
                   << "' en " << ref_result.execution_time_ms << " ms\n\n";
    } else {
        std::cout << "  ERROR: no se pudo validar la instancia de referencia.\n\n";
        return;
    }

     // 2. Alfabetos y orden de la práctica
    std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::vector<std::string> alfabetos_equipo = {A1, A2, A1, A2, A1};

    int longitudes[5] = {4, 4, 5, 5, 6};

    // 3. Apellidos del equipo concatenados ordenados alfabéticamente

    std::string apellidos_juntos = "cifuentesmartinezposada"; 

    long long semilla = calcular_semilla(apellidos_juntos);

    std::cout << "Semilla del equipo (cifuentes, martinez, posada): " << semilla << "\n\n";

    // Generar las 5 contraseñas reales con el LCG
    std::vector<std::string> passwords_equipo = generar_passwords_equipo(semilla, alfabetos_equipo);

    // 4. Ejecución de las 5 instancias (Fuerza Bruta vs Diccionario)
    std::vector<FBResult> resultados_fb;
    std::string ruta_diccionario = "resources/diccionario.txt";

    std::cout << "=== Ejecutando las 5 Instancias del Equipo ===\n";

    for (int i = 0; i < 5; ++i) {

        std::string pass_real = passwords_equipo[i];

        std::string hash_objetivo = picosha2::hash256_hex_string(pass_real);

        std::string alf_actual = alfabetos_equipo[i];

        int len_actual = longitudes[i];

        std::string nombre_alf = (i == 1 || i == 3) ? "A2" : "A1";

        std::cout << "Instancia " << (i + 1) << " | Longitud: " << len_actual 

                  << " | Alfabeto: " << nombre_alf << "\n";

        // Ejecutar Fuerza Bruta Pura (retorna FBResult)
        FBResult res_fb = search_by_brute_force(hash_objetivo, alf_actual, len_actual);
        resultados_fb.push_back(res_fb);

        std::cout << "  -> Fuerza Bruta: " << (res_fb.found ? "Encontrada (" + res_fb.password + ")" : "NO encontrada")

                  << " | Tiempo: " << res_fb.execution_time_ms << " ms"

                  << " | Evaluados: " << res_fb.candidates_evaluated << "\n";

        // Comparación opcional con Diccionario
        FBResult res_dic = search_by_dictionary(hash_objetivo, ruta_diccionario);

        std::cout << "  -> Diccionario : " << (res_dic.found ? "Encontrada (" + res_dic.password + ")" : "NO encontrada")

                  << " | Tiempo: " << res_dic.execution_time_ms << " ms\n\n";
    }
    // 5. Guardar resultados en el archivo CSV
    export_fb_results_to_csv(resultados_fb, "results/fb_resultados.csv");
    std::cout << "[EXITO] Experimentos finalizados. Archivo 'results/fb_resultados.csv' generado.\n";
}


void run_bt_module() {
    std::cout << "=== Modulo Backtracking ===\n";
    std::cout << ".\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: ./ada_p1 --modo fb | --modo bt\n";
        return 1;
    }

    std::string modo = argv[1];

    if (modo == "--modo" && argc >= 3) {
        std::string submodo = argv[2];
        if (submodo == "fb") {
            run_fb_module();
        } else if (submodo == "bt") {
            run_bt_module();
        } else {
            std::cout << "Modo desconocido: " << submodo << "\n";
            return 1;
        }
    } else {
        std::cout << "Uso: ./ada_p1 --modo fb | --modo bt\n";
        return 1;
    }

    return 0;
}