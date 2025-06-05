/**
 * @file Parser.h
 * @brief Analizador sintáctico para el lenguaje Setker
 * @author Javier
 * @date 2025
 * 
 * Este archivo define el analizador sintáctico (parser) que convierte
 * la secuencia de tokens en un Árbol de Sintaxis Abstracta (AST).
 * Implementa un parser recursivo descendente con precedencia de operadores.
 */

#ifndef PARSER_H
#define PARSER_H

#include <memory_resource>
#include "../def/Tokens.h"
#include "../def/ASTNode.h"
#include "../def/ErrorCode.h"

using namespace TokenTree;
using Error = TokenTree::Error;

/**
 * @namespace Parser
 * @brief Espacio de nombres para el analizador sintáctico
 * 
 * Contiene todas las funciones necesarias para realizar el análisis
 * sintáctico de tokens y construcción del AST según la gramática
 * del lenguaje Setker.
 */
namespace Parser {
    /**
     * @brief Analiza tokens y muestra el AST resultante
     * @param tokens Vector de tokens a analizar
     * @return int Código de salida (0 = éxito, 65 = error sintáctico)
     * 
     * Realiza el análisis sintáctico completo de los tokens proporcionados,
     * construye el AST y lo imprime en la salida estándar.
     * 
     * Esta función es utilizada por el comando 'parse' del intérprete.
     */
    int parse(const std::pmr::vector<Token>& tokens);
    
    /**
     * @brief Construye el AST sin imprimir
     * @param tokens Vector de tokens a analizar
     * @return std::unique_ptr<ASTNode> Puntero al nodo raíz del AST
     * @throws Error Si encuentra errores sintácticos
     * 
     * Realiza el análisis sintáctico de los tokens y devuelve el AST
     * resultante sin imprimirlo. Esta función es utilizada por otras
     * fases del intérprete como evaluación y ejecución.
     * 
     * Implementa un parser recursivo descendente que respeta la
     * precedencia y asociatividad de operadores del lenguaje.
     */
    std::unique_ptr<ASTNode> parseAST(const std::pmr::vector<Token>& tokens);
}

#endif // PARSER_H
