#include <iostream>
#include <iomanip>
#include "../src/bt_solver.hpp"

int main() {
    // Alfabeto base completo de 69 símbolos
    std::string alphabet = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%";

    // Instancia de Referencia Común (Sección 9.2 de la guía)
    Policy refPolicy = {
        6,    // n = 6
        2,    // minLower = 2
        1,    // minUpper = 1
        1,    // minDigit = 1
        1,    // minSymbol = 1
        true  // sin repetidos consecutivos
    };

    std::cout << "==========================================\n";
    std::cout << " Pruebas de Referencia - Modulo Backtracking\n";
    std::cout << " Instancia: n = 6 | Alfabeto = 69 simbolos\n";
    std::cout << "==========================================\n\n";

    BTSolver solver(alphabet, refPolicy);

    std::cout << "Ejecutando BT CON Poda...\n";
    BTMetrics mWith = solver.solveWithPruning();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n---------------- RESULTADOS ------------------\n";
    std::cout << "Soluciones encontradas: " << mWith.solutionsFound << "\n";
    std::cout << "Nodos visitados:        " << mWith.nodesGenerated << "\n";
    std::cout << "Nodos podados:          " << mWith.nodesPruned << "\n";
    std::cout << "Tiempo de ejecucion:    " << mWith.executionTimeMs << " ms\n";
    std::cout << "==============================================\n";

    return 0;
}