/**
 * @file Tokenizer.h
 * @brief Analizador léxico para el lenguaje Setker
 * @author Javier
 * @date 2025
 * 
 * Este archivo define el analizador léxico (tokenizer) que convierte
 * el código fuente en una secuencia de tokens. Es la primera fase
 * del proceso de compilación/interpretación.
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <memory_resource>

#include "../def/Tokens.h"

/**
 * @namespace Tokenizer
 * @brief Espacio de nombres para el analizador léxico
 * 
 * Contiene todas las funciones y estructuras necesarias para
 * realizar el análisis léxico del código fuente de Setker.
 */
namespace Tokenizer {
    /**
     * @struct Result
     * @brief Resultado del proceso de tokenización
     * 
     * Estructura que encapsula el resultado del análisis léxico,
     * incluyendo la lista de tokens generados y el código de salida
     * que indica si hubo errores durante el proceso.
     */
    struct Result {
        std::pmr::vector<Token> tokens;  ///< Vector de tokens identificados
        int exitCode;                    ///< Código de salida (0 = éxito, >0 = error)
    };

    /**
     * @brief Tokeniza y muestra el contenido de un archivo
     * @param file_contents Contenido del archivo a tokenizar
     * @return int Código de salida (0 = éxito, 65 = error léxico)
     * 
     * Realiza el análisis léxico completo del contenido proporcionado,
     * imprime todos los tokens identificados en la salida estándar
     * y devuelve un código de salida indicando el resultado.
     * 
     * Esta función es utilizada por el comando 'tokenize' del intérprete.
     */
    int tokenize(const std::string& file_contents);
    
    /**
     * @brief Tokeniza contenido y devuelve tokens sin imprimir
     * @param file_contents Contenido del archivo a tokenizar
     * @return Result Estructura con tokens y código de salida
     * 
     * Realiza el análisis léxico del contenido proporcionado y devuelve
     * los tokens en una estructura Result junto con el código de salida.
     * No imprime los tokens, permitiendo su uso en otras fases del
     * procesamiento como parsing y evaluación.
     */
    Result getTokens(const std::string& file_contents);
}

#endif // TOKENIZER_H