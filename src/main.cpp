#include "third_party/picosha2.h"
#include "fb_ejercicio.hpp"
#include "bt_solver.hpp"
#include "bt_dp.hpp"
#include <iostream>
#include <string>

void run_fb_module() {
    std::cout << "=== Modulo Fuerza Bruta ===\n\n";

    // Instancia de referencia oficial 
    std::string alphabet_ref = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string hash_ref = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

    std::cout << "Buscando password para hash conocido...\n";
    FBResult ref_result = search_by_brute_force(hash_ref, alphabet_ref, 5);

    if (ref_result.found && ref_result.password == "abc12") {
        std::cout << "  OK: encontrada '" << ref_result.password
                   << "' en " << ref_result.execution_time_ms << " ms\n\n";
    } else {
        std::cout << "  ERROR: no se pudo validar la instancia de referencia.\n\n";
        return;
    }

     // Alfabetos
    std::string A1 = "abcdefghijklmnopqrstuvwxyz";
    std::string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::vector<std::string> alfabetos_equipo = {A1, A2, A1, A2, A1};

    int longitudes[5] = {4, 4, 5, 5, 6};

    std::string apellidos_juntos = "cifuentesmartinezposada"; 

    long long semilla = calcular_semilla(apellidos_juntos);

    std::cout << "Semilla del equipo (Cifuentes, Martinez, Posada): " << semilla << "\n\n";

    // Generar las 5 contraseñas reales con el LCG
    std::vector<std::string> passwords_equipo = generar_passwords_equipo(semilla, alfabetos_equipo);

    // Ejecucion de Fuerza Bruta
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
    // Guardar resultados en el archivo CSV
    export_fb_results_to_csv(resultados_fb, "results/fb_resultados.csv");
    std::cout << "[EXITO] Experimentos finalizados. Archivo 'results/fb_resultados.csv' generado.\n";
}

void run_bt_module() {
    std::cout << "=== Modulo Backtracking ===\n\n";

    std::string alphabet =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    std::cout << std::fixed;
    std::cout.precision(3);

    // Generar la semilla

    std::string apellidos_juntos = "cifuentesmartinezposada";
    long long semilla = calcular_semilla(apellidos_juntos);

    int pol_minLower = 2 + (semilla % 3);
    int pol_minUpper = 1 + (semilla % 2);
    int pol_minDigit = 1 + (semilla % 3);
    int pol_minSymbol = 1;

    std::cout << "Semilla del equipo: " << semilla << "\n";
    std::cout << "Politica original: minLower=" << pol_minLower 
              << ", minUpper=" << pol_minUpper 
              << ", minDigit=" << pol_minDigit 
              << ", minSymbol=" << pol_minSymbol << "\n\n";

    // Función auxiliar para correr y medir una instancia
    auto ejecutar_instancia = [&](const std::string& nombre, const Policy& p, bool correr_sin_poda) {
        std::cout << "--- " << nombre << " (n=" << p.length << ") ---\n";
        
        BTSolver solver(alphabet, p);
        long long conteo_dp = countSolutionsDP(alphabet, p);
        
        std::cout << "Soluciones exactas (DP): " << conteo_dp << "\n";

        // Ejecutar Con Poda
        std::cout << "Ejecutando con Poda...\n";
        BTMetrics mCon = solver.solveWithPruning();
        std::cout << "  Nodos generados : " << mCon.nodesGenerated << "\n"
                  << "  Nodos podados   : " << mCon.nodesPruned << "\n"
                  << "  Soluciones      : " << mCon.solutionsFound << "\n"
                  << "  Tiempo          : " << mCon.executionTimeMs << " ms\n";

        // Ejecutar sin Poda
        if (correr_sin_poda) {
            std::cout << "Ejecutando sin Poda...\n";
            BTMetrics mSin = solver.solveWithoutPruning();
            std::cout << "  Nodos generados : " << mSin.nodesGenerated << "\n"
                      << "  Soluciones      : " << mSin.solutionsFound << "\n"
                      << "  Tiempo          : " << mSin.executionTimeMs << " ms\n";
            
            double reduccion = (1.0 - (double)mCon.nodesGenerated / (double)mSin.nodesGenerated) * 100.0;
            std::cout << "  Reduccion del espacio de busqueda: " << reduccion << " %\n";
        }
        std::cout << "\n";
    };

    Policy pRef = {6, 2, 1, 1, 1, true};
    ejecutar_instancia("0. Instancia de Referencia", pRef, true); // True: Correr con y sin poda

    // Preparación para las variantes del equipo: Ajuste de minLower si supera n
    int minLower_n8 = pol_minLower;
    if ((minLower_n8 + pol_minUpper + pol_minDigit + pol_minSymbol) > 8) {
        minLower_n8 -= ((minLower_n8 + pol_minUpper + pol_minDigit + pol_minSymbol) - 8);
    }
    
    int minLower_n6 = pol_minLower;
    if ((minLower_n6 + pol_minUpper + pol_minDigit + pol_minSymbol) > 6) {
        minLower_n6 -= ((minLower_n6 + pol_minUpper + pol_minDigit + pol_minSymbol) - 6);
    }


    Policy p1 = {8, minLower_n8, pol_minUpper, pol_minDigit, pol_minSymbol, true};
    ejecutar_instancia("Politica completa del equipo", p1, false); // False: Solo con poda 

    // n=6
    Policy p2 = {6, minLower_n6, pol_minUpper, pol_minDigit, pol_minSymbol, true};
    ejecutar_instancia("n=6", p2, true); // True: Correr con y sin poda para comparar

    // n=10
    Policy p3 = {10, pol_minLower, pol_minUpper, pol_minDigit, pol_minSymbol, true};
    ejecutar_instancia("n=10", p3, false); // False: Solo con poda

    // Política relajada, n=8
    Policy p4 = {8, 1, 0, 0, 0, false};
    ejecutar_instancia("Politica relajada, n=8", p4, false); // False: Solo con poda

    // Política sin restricciones, n=6
    Policy p5 = {6, 0, 0, 0, 0, false};
    ejecutar_instancia("Politica sin restricciones (Poda nula)", p5, true); // True: Para calibrar la poda nula

    std::cout << "[EXITO] Modulo Backtracking finalizado.\n";
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
