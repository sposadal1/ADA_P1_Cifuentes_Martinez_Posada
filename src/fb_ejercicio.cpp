#include "fb_ejercicio.hpp"
#include "third_party/picosha2.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdint>
// Función auxiliar para calcular el hash SHA-256 usando picosha2
std::string calculate_sha256(const std::string& input) {

    return picosha2::hash256_hex_string(input);
}
FBResult search_by_brute_force(
    const std::string& target_hash, 
                                
    const std::string& alphabet, 
                                
    int length) {

    auto start_time = std::chrono::high_resolution_clock::now();
    
    FBResult result;

    result.found = false;

    result.candidates_evaluated = 0;
    
    size_t base = alphabet.size();

   

    

        // Representamos el estado actual usando los índices de los caracteres en el alfabeto.

        // indices[0] es el carácter más a la izquierda, indices[len-1] el más a la derecha.

        std::vector<size_t> indices(length, 0);
        
        // Calculamos el número total de combinaciones posibles para esta longitud: base^len
        long long combinations = 1;
        for (int i = 0; i < length; ++i) {
            combinations *= base;
        }
        // Iteramos sistemáticamente por todas las combinaciones de longitud 'len'
        for (long long c = 0; c < combinations; ++c) {
            // Construimos la cadena candidata a partir de los índices actuales
            std::string candidate = "";
            for (int i = 0; i < length; ++i) {
                candidate += alphabet[indices[i]];
            }
            result.candidates_evaluated++;
            // Evaluamos si el hash coincide
            if (calculate_sha256(candidate) == target_hash) {

                result.found = true;

                result.password = candidate;
                break;
            }
            // Incrementamos el contador posicional (aritmética en base |alphabet|)
            
            for (int i = length - 1; i >= 0; --i) {
                indices[i]++;
                if (indices[i] < base) {
                    break; // No hay llevada, terminamos de incrementar
                } else {
                    indices[i] = 0; // Hay llevada, este dígito vuelve a 0 e incrementa el siguiente
                }
            }
        }
        
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, 

    std::milli> elapsed = end_time - start_time;

    result.execution_time_ms = elapsed.count();
    
    result.length_used = length;

    return result;
}
FBResult search_by_dictionary(
    const std::string& target_hash, 
                               
    const std::string& dictionary_path) {

    auto start_time = std::chrono::high_resolution_clock::now();
    
    FBResult result;

    result.found = false;

    result.candidates_evaluated = 0;

    std::ifstream file(dictionary_path);

    if (!file.is_open()) {

        std::cerr << "Error: No se pudo abrir el archivo de diccionario " << dictionary_path << std::endl;

        result.execution_time_ms = 0;

        return result;

    }
    std::string candidate;

    while (std::getline(file, candidate)) {

        // Limpiar retornos de carro si el archivo proviene de Windows (\r\n)
        if (!candidate.empty() && candidate.back() == '\r') {

            candidate.pop_back();
        }

        result.candidates_evaluated++;

        if (calculate_sha256(candidate) == target_hash) {

            result.found = true;

            result.password = candidate;

            break;
        }
    }
    file.close();

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    result.execution_time_ms = elapsed.count();
    
    return result;
}

long long calcular_semilla(const std::string& apellidos_concatenados) {
    long long suma = 0;
    for (char c : apellidos_concatenados) suma += static_cast<unsigned char>(c);
    return suma % 100000;
}

std::vector<std::string> generar_passwords_equipo(
    long long semilla,
    const std::vector<std::string>& alfabetos_por_posicion) {

    if (alfabetos_por_posicion.size() != 5) return {};

    int longitudes[5] = {4, 4, 5, 5, 6};
    uint64_t x = static_cast<uint64_t>(semilla);
    const uint64_t a = 1103515245, c = 12345, mod = 1ULL << 31;

    std::vector<std::string> passwords;
    for (int p = 0; p < 5; ++p) {
        std::string password;
        const std::string& alfabeto = alfabetos_por_posicion[p];
        for (int i = 0; i < longitudes[p]; ++i) {
            x = (a * x + c) % mod;
            password += alfabeto[x % alfabeto.size()];
        }
        passwords.push_back(password);
    }
    return passwords;
}
std::vector<FBResult> search_by_brute_force_range(
    const std::string& target_hash,
    const std::string& alphabet,
    int min_length,
    int max_length) {

    std::vector<FBResult> resultados;
    for (int len = min_length; len <= max_length; ++len) {
        FBResult r = search_by_brute_force(target_hash, alphabet, len);
        resultados.push_back(r);
        if (r.found) break;
    }
    return resultados;
}

void export_fb_results_to_csv(
    const std::vector<FBResult>& results,
    const std::string& output_path) {

    std::ofstream out(output_path);
    if (!out.is_open()) {
        std::cerr << "Error: no se pudo crear el archivo CSV " << output_path << std::endl;
        return;
    }

    out << "length_used,found,password,candidates_evaluated,execution_time_ms\n";
    for (const auto& r : results) {
        out << r.length_used << ","
            << (r.found ? "true" : "false") << ","
            << r.password << ","
            << r.candidates_evaluated << ","
            << r.execution_time_ms << "\n";
    }

    out.close();
}

FBComparisonResult compare_brute_force_vs_dictionary(
    const std::string& target_hash,
    const std::string& alphabet,
    int length,
    const std::string& dictionary_path) {

    FBComparisonResult comparison;
    comparison.brute_force_result = search_by_brute_force(target_hash, alphabet, length);
    comparison.dictionary_result = search_by_dictionary(target_hash, dictionary_path);
    return comparison;
}