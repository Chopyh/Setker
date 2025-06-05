/**
 * @file Keywords.cpp
 * @brief Implementación del sistema de reconocimiento de palabras clave
 * @author Javier Castillo
 * @date 2024
 * 
 * Este archivo implementa el reconocimiento y clasificación de palabras clave
 * del lenguaje Setker. Las palabras clave son identificadores reservados que
 * tienen significado especial en el lenguaje.
 * 
 * Características:
 * - Definición de constantes para todas las palabras clave
 * - Función de valoración que convierte strings a tipos de token
 * - Soporte para palabras clave de control de flujo, tipos de datos y operadores
 */

#include "Keywords.h"

// Definición de constantes para palabras clave del lenguaje Setker
// Utilizamos string_view para evitar copias innecesarias y mejorar rendimiento
static constexpr std::string_view AND = "and";      ///< Operador lógico AND
static constexpr std::string_view CLASS = "class";  ///< Declaración de clase
static constexpr std::string_view FALSE = "false";  ///< Literal booleano falso
static constexpr std::string_view NIL = "nil";      ///< Valor nulo/vacío
static constexpr std::string_view OR = "or";        ///< Operador lógico OR
static constexpr std::string_view PRINT = "print";  ///< Función de impresión
static constexpr std::string_view SUPER = "super";  ///< Referencia a superclase
static constexpr std::string_view THIS = "this";    ///< Referencia a instancia actual
static constexpr std::string_view TRUE = "true";    ///< Literal booleano verdadero
static constexpr std::string_view FUN = "fun";      ///< Declaración de función

/**
 * @brief Evalúa si una cadena corresponde a una palabra clave del lenguaje
 * 
 * Esta función examina la cadena proporcionada y determina si corresponde
 * a alguna palabra clave reservada del lenguaje Setker. Si es así, retorna
 * el tipo de token correspondiente; si no, retorna IDENTIFIER.
 * 
 * Palabras clave soportadas:
 * - Operadores lógicos: and, or
 * - Literales: true, false, nil
 * - Control de flujo: if, else, for, while
 * - Declaraciones: var, fun, class, return
 * - Referencias: this, super
 * - Funciones built-in: print
 * 
 * @param keyword String a evaluar como posible palabra clave
 * @return TokenType El tipo de token correspondiente:
 *         - Tipo específico si es una palabra clave reconocida
 *         - TokenType::IDENTIFIER si no es una palabra clave
 * 
 * @note La función utiliza comparaciones de string eficientes con string_view
 * @note Es case-sensitive: solo reconoce palabras clave en minúsculas
 */
TokenType Keywords::valorateKeyword(std::string keyword) {
    // Operadores lógicos
    if (keyword == AND) return TokenType::AND;
    if (keyword == OR) return TokenType::OR;
    
    // Declaraciones de estructura
    if (keyword == CLASS) return TokenType::CLASS;
    if (keyword == FUN) return TokenType::FUN;
    if (keyword == VAR) return TokenType::VAR;
    
    // Control de flujo
    if (keyword == IF) return TokenType::IF;
    if (keyword == ELSE) return TokenType::ELSE;
    if (keyword == FOR) return TokenType::FOR;
    if (keyword == WHILE) return TokenType::WHILE;
    if (keyword == RETURN) return TokenType::RETURN;
    
    // Literales y valores especiales
    if (keyword == TRUE) return TokenType::TRUE;
    if (keyword == FALSE) return TokenType::FALSE;
    if (keyword == NIL) return TokenType::NIL;
    
    // Referencias de objeto
    if (keyword == THIS) return TokenType::THIS;
    if (keyword == SUPER) return TokenType::SUPER;
    
    // Funciones built-in
    if (keyword == PRINT) return TokenType::PRINT;
    
    // Si no es una palabra clave, es un identificador
    return TokenType::IDENTIFIER;
}
