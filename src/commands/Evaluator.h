/**
 * @file Evaluator.h
 * @brief Evaluador de expresiones y ejecutor de instrucciones
 * @author Javier
 * @date 2025
 * 
 * Este archivo define el evaluador que recorre el AST y ejecuta las
 * instrucciones del programa, manejando variables, funciones, expresiones
 * aritméticas y estructuras de control del lenguaje Setker.
 */

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory_resource>
#include <vector>
#include "../def/Tokens.h"
#include "../def/ASTNode.h"
#include "../def/Environment.h"
#include "../def/ErrorCode.h"

/**
 * @namespace Evaluator
 * @brief Espacio de nombres para el evaluador de expresiones
 * 
 * Contiene todas las estructuras y funciones necesarias para evaluar
 * el AST generado por el parser, ejecutando las instrucciones del
 * programa y manejando el estado de ejecución.
 */
namespace Evaluator {
    using Error = TokenTree::Error;
    
    /**
     * @typedef Value
     * @brief Alias para el tipo de valor utilizado en evaluación
     * 
     * Representa cualquier valor que puede ser almacenado y manipulado
     * durante la ejecución del programa.
     */
    using Value = ::TokenTree::Environment::Value;

    /**
     * @struct LoxFunction
     * @brief Representación de funciones definidas por el usuario
     * 
     * Estructura que encapsula toda la información necesaria para
     * representar y ejecutar funciones definidas en el código fuente,
     * incluyendo soporte para closures y captura de entorno.
     */
    struct LoxFunction { 
        std::string name;                               ///< Nombre de la función
        std::vector<std::string> params;               ///< Lista de parámetros
        const TokenTree::ASTNode* body;                ///< Cuerpo de la función (AST)
        std::shared_ptr<TokenTree::Environment> closure; ///< Entorno capturado (closure)
        
        /**
         * @brief Constructor de LoxFunction
         * @param name Nombre de la función
         * @param params Vector de nombres de parámetros
         * @param body Puntero al nodo AST del cuerpo de la función
         * @param closure Entorno capturado para closures
         */
        LoxFunction(std::string name, std::vector<std::string> params, 
                   const TokenTree::ASTNode* body, std::shared_ptr<TokenTree::Environment> closure) 
            : name(std::move(name)), params(std::move(params)), body(body), closure(closure) {} 
    };
    
    /**
     * @struct ReturnException
     * @brief Excepción interna para manejar instrucciones return
     * 
     * Se utiliza como mecanismo de control de flujo interno para
     * implementar el comportamiento de la instrucción return en funciones.
     * No es un error, sino una forma controlada de terminar la ejecución
     * de una función y devolver un valor.
     */
    struct ReturnException {
        Value value; ///< Valor retornado por la función
        
        /**
         * @brief Constructor de ReturnException
         * @param v Valor a retornar
         */
        explicit ReturnException(const Value& v) : value(v) {}
    };

    /**
     * @typedef FunctionPtr
     * @brief Puntero compartido a función definida por el usuario
     */
    using FunctionPtr = std::shared_ptr<LoxFunction>;

    /**
     * @brief Evalúa tokens y muestra el resultado
     * @param tokens Vector de tokens a evaluar
     * @return int Código de salida (0 = éxito, >0 = error)
     * 
     * Combina parsing y evaluación para mostrar el resultado de
     * evaluar una expresión. Utilizado por el comando 'evaluate'.
     */
    int evaluate(const std::pmr::vector<Token>& tokens);
    
    /**
     * @brief Evalúa un nodo del AST directamente
     * @param node Puntero al nodo AST a evaluar
     * @return Value Resultado de la evaluación
     * @throws Error Si ocurre un error durante la evaluación
     * @throws ReturnException Si se ejecuta una instrucción return
     * 
     * Función principal de evaluación que recorre recursivamente
     * el AST ejecutando las instrucciones y evaluando expresiones.
     * Maneja todos los tipos de nodos del AST incluyendo:
     * - Literales (números, cadenas, booleanos)
     * - Operaciones aritméticas y lógicas
     * - Variables y asignaciones
     * - Funciones y llamadas
     * - Estructuras de control (if, while, for)
     * - Instrucciones (print, return)
     */
    Value evalNode(const TokenTree::ASTNode* node);
}

#endif // EVALUATOR_H
