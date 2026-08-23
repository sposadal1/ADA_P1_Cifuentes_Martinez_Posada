#include "../src/fb_ejercicio.hpp"
#include <iostream>

int main() {
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string target_hash = "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";

    auto resultados = search_by_brute_force_range(target_hash, alphabet, 3, 6);
    export_fb_results_to_csv(resultados, "results/fb_test_export.csv");

    std::cout << "CSV generado en results/fb_test_export.csv\n";
    return 0;
}