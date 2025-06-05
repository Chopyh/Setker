/**
 * @file Keywords.h
 * @brief Definición y reconocimiento de palabras clave del lenguaje
 * @author Javier
 * @date 2025
 * 
 * Este archivo define todas las palabras clave (keywords) reconocidas
 * por el lenguaje Setker y proporciona funcionalidad para distinguir
 * entre identificadores y palabras reservadas durante el análisis léxico.
 */

#ifndef KEWYORDS_H
#define KEWYORDS_H

#include <string>
#include "Tokens.h"
#include <string_view>

/**
 * @class Keywords
 * @brief Manejo de palabras clave del lenguaje Setker
 * 
 * Proporciona constantes para todas las palabras clave del lenguaje
 * y funcionalidad para determinar si un identificador es una palabra
 * reservada durante el proceso de tokenización.
 */
class Keywords {
public:
    // Palabras clave principales del lenguaje
    static constexpr std::string_view VAR = "var";           ///< Declaración de variable
    static constexpr std::string_view IF = "if";             ///< Condicional if
    static constexpr std::string_view ELSE = "else";         ///< Alternativa else
    static constexpr std::string_view WHILE = "while";       ///< Bucle while
    static constexpr std::string_view FOR = "for";           ///< Bucle for
    static constexpr std::string_view FUNCTION = "fn";       ///< Declaración de función (alias)
    static constexpr std::string_view RETURN = "return";     ///< Retorno de función

    /**
     * @brief Evalúa si un identificador es una palabra clave
     * @param keyword Cadena a evaluar
     * @return TokenType Tipo de token correspondiente
     * 
     * Determina si la cadena proporcionada corresponde a una palabra
     * clave del lenguaje. Si es así, devuelve el TokenType apropiado,
     * de lo contrario devuelve TokenType::IDENTIFIER.
     * 
     * Esta función es utilizada por el tokenizer para distinguir
     * entre identificadores definidos por el usuario y palabras
     * reservadas del lenguaje.
     */
    static TokenType valorateKeyword(std::string keyword);
};

#endif //KEWYORDS_H
