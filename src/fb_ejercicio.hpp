#ifndef FB_EJERCICIO_HPP
#define FB_EJERCICIO_HPP
#include <string>
#include <vector>

// Estructura para almacenar las métricas de la ejecución
struct FBResult {
    bool found;
    std::string password;
    long long candidates_evaluated;
    double execution_time_ms;
};

/**
 * Realiza una búsqueda exhaustiva (Fuerza Bruta) de una contraseña a partir de su hash SHA-256.
 * 
 * @param target_hash Hash SHA-256 objetivo en formato hexadecimal.
 * @param alphabet El conjunto de caracteres a utilizar (ej. "abcdefghijklmnopqrstuvwxyz").
 * @param length La longitud exacta de contraseña a intentar.
 * @return Estructura FBResult con el resultado y métricas.
 */
FBResult search_by_brute_force(
    const std::string& target_hash,
    const std::string& alphabet,
    int length);

/**
 * Realiza un ataque basado en un archivo de diccionario.
 * 
 * @param target_hash Hash SHA-256 objetivo en formato hexadecimal.
 * @param dictionary_path Ruta al archivo de texto con las palabras.
 * @return Estructura FBResult con el resultado y métricas.
 */
FBResult search_by_dictionary(
    const std::string& target_hash,
    const std::string& dictionary_path);

#endif 