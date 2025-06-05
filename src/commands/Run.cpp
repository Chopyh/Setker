/**
 * @file Run.cpp
 * @brief Implementación del ejecutor principal del intérprete Setker
 * @author Javier Castillo
 * @date 2024
 * 
 * Este archivo contiene la implementación del módulo Run, que orquesta
 * todo el proceso de ejecución de un programa Setker desde los tokens
 * hasta la evaluación final.
 * 
 * El proceso incluye:
 * - Análisis sintáctico (parsing) de tokens a AST
 * - Evaluación del AST resultante
 * - Manejo de errores y códigos de salida
 */

#include "Run.h"
#include "Parser.h"
#include "Evaluator.h"
#include "../def/ErrorCode.h"
#include <iostream>
#include <memory_resource>

namespace Run {
    /**
     * @brief Ejecuta un programa Setker a partir de su secuencia de tokens
     * 
     * Esta función implementa el pipeline completo de ejecución:
     * 1. Convierte los tokens en un Árbol de Sintaxis Abstracta (AST)
     * 2. Evalúa el AST para ejecutar el programa
     * 3. Maneja errores y retorna códigos de salida apropiados
     * 
     * @param tokens Vector de tokens que representa el programa a ejecutar
     * @return int Código de salida del programa:
     *         - 0: Ejecución exitosa
     *         - Código específico de error: Según el tipo de error encontrado
     *         - 1: Error general o excepción no controlada
     * 
     * @throws std::exception Para errores inesperados del sistema
     * 
     * Flujo de ejecución:
     * - Parser::parseAST() convierte tokens en AST
     * - Evaluator::evalNode() ejecuta el programa
     * - Los errores de evaluación se capturan y reportan con código específico
     * - Las excepciones no controladas se reportan con código genérico
     */
    int run(const std::pmr::vector<Token>& tokens) {
        try {
            // Usar el AST del parser - Convierte tokens a estructura de árbol
            auto ast = Parser::parseAST(tokens);
            
            // Evaluar el AST - Ejecuta el programa representado por el árbol
            // Soporta declaraciones print, expresiones aritméticas y lógicas
            Evaluator::evalNode(ast.get());
            
            return 0; // Ejecución exitosa
        } catch (const Evaluator::Error& e) {
            // Error específico del evaluador - reportar mensaje y código
            std::cerr << e.message << std::endl;
            return e.type.code;
        } catch (const std::exception& e) {
            // Error general del sistema - reportar mensaje genérico
            std::cerr << e.what() << std::endl;
            return 1; // Código de error genérico
        }
    }
}
