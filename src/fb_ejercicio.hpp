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
    int length_used; 
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


    // Calcula la semilla del equipo 
long long calcular_semilla(const std::string& apellidos_concatenados);

// Genera las 5 contraseñas del equipo. 
std::vector<std::string> generar_passwords_equipo(
    long long semilla,
    const std::vector<std::string>& alfabetos_por_posicion);

    // Prueba longitudes desde min_length hasta max_length, deteniendose al encontrar.
// Devuelve un resultado por cada longitud efectivamente probada.
std::vector<FBResult> search_by_brute_force_range(
    const std::string& target_hash,
    const std::string& alphabet,
    int min_length,
    int max_length);
    // Exporta una lista de resultados FB a un archivo CSV.

void export_fb_results_to_csv(
    const std::vector<FBResult>& results,
    const std::string& output_path);
    // Compara fuerza bruta vs. diccionario para el mismo hash objetivo (Seccion 8.1).
struct FBComparisonResult {
    FBResult brute_force_result;
    FBResult dictionary_result;
};

FBComparisonResult compare_brute_force_vs_dictionary(
    const std::string& target_hash,
    const std::string& alphabet,
    int length,
    const std::string& dictionary_path);
#endif 

