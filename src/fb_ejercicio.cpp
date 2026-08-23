#include "fb_ejercicio.hpp"
#include "third_party/picosha2.h"
#include <chrono>
#include <fstream>
#include <iostream>
// Función auxiliar para calcular el hash SHA-256 usando picosha2
std::string calculate_sha256(const std::string& input) {

    return picosha2::hash256_hex_string(input);
}
FBResult search_by_brute_force(
    const std::string& target_hash, 
                                
    const std::string& alphabet, 
                                
    int max_length) {

    auto start_time = std::chrono::high_resolution_clock::now();
    
    FBResult result;

    result.found = false;

    result.candidates_evaluated = 0;
    
    size_t base = alphabet.size();

    // Probamos cada longitud k desde 1 hasta max_length

    for (int len = 1; len <= max_length; ++len) {

        // Representamos el estado actual usando los índices de los caracteres en el alfabeto.

        // indices[0] es el carácter más a la izquierda, indices[len-1] el más a la derecha.

        std::vector<size_t> indices(len, 0);
        
        // Calculamos el número total de combinaciones posibles para esta longitud: base^len
        long long combinations = 1;
        for (int i = 0; i < len; ++i) {
            combinations *= base;
        }
        // Iteramos sistemáticamente por todas las combinaciones de longitud 'len'
        for (long long c = 0; c < combinations; ++c) {
            // Construimos la cadena candidata a partir de los índices actuales
            std::string candidate = "";
            for (int i = 0; i < len; ++i) {
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
            
            for (int i = len - 1; i >= 0; --i) {
                indices[i]++;
                if (indices[i] < base) {
                    break; // No hay llevada, terminamos de incrementar
                } else {
                    indices[i] = 0; // Hay llevada, este dígito vuelve a 0 e incrementa el siguiente
                }
            }
        }
        if (result.found) {
            break;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    result.execution_time_ms = elapsed.count();
    
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