/**
 * @file Environment.h
 * @brief Sistema de entornos para manejo de variables y scope
 * @author Javier
 * @date 2025
 * 
 * Este archivo define la clase Environment que maneja el almacenamiento
 * y resolución de variables en diferentes ámbitos (scopes), implementando
 * un sistema de entornos anidados para soportar scoping léxico.
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include "ASTNode.h"

// Forward declaration para evitar dependencias circulares
namespace Evaluator { 
    struct LoxFunction; 
    using FunctionPtr = std::shared_ptr<LoxFunction>; 
}

namespace TokenTree {
    /**
     * @class Environment
     * @brief Entorno de ejecución para variables y funciones
     * 
     * Implementa un sistema de entornos anidados que permite el manejo
     * de variables locales y globales, soportando scoping léxico y
     * closures. Cada entorno puede tener un entorno padre (enclosing).
     */
    class Environment {
    public:
        /**
         * @typedef Value
         * @brief Tipo variante para valores almacenados en el entorno
         * 
         * Puede contener:
         * - std::monostate: Valor nil/vacío
         * - double: Números
         * - bool: Valores booleanos
         * - std::string: Cadenas de texto
         * - Evaluator::FunctionPtr: Funciones definidas por el usuario
         */
        using Value = std::variant<std::monostate, double, bool, std::string, Evaluator::FunctionPtr>;
        
        /**
         * @brief Constructor para entorno global (sin padre)
         */
        Environment();
        
        /**
         * @brief Constructor para entorno anidado
         * @param enclosing Puntero al entorno padre
         */
        Environment(std::shared_ptr<Environment> enclosing);
        
        /**
         * @brief Define una nueva variable en el entorno actual
         * @param name Nombre de la variable
         * @param value Valor de la variable
         * 
         * Crea una nueva variable en el entorno actual. Si la variable
         * ya existe, sobrescribe su valor.
         */
        void define(const std::string& name, const Value& value);
        
        /**
         * @brief Obtiene el valor de una variable
         * @param name Nombre de la variable
         * @return Value Valor de la variable
         * @throws std::runtime_error Si la variable no está definida
         * 
         * Busca la variable primero en el entorno actual, luego en
         * entornos padre hasta encontrarla o lanzar error.
         */
        Value get(const std::string& name) const;
        
        /**
         * @brief Asigna un nuevo valor a una variable existente
         * @param name Nombre de la variable
         * @param value Nuevo valor
         * @throws std::runtime_error Si la variable no está definida
         * 
         * Busca la variable en el entorno actual y entornos padre,
         * asignando el nuevo valor en el entorno donde se encuentra.
         */
        void assign(const std::string& name, const Value& value);
        
    private:
        std::unordered_map<std::string, Value> values;  ///< Variables del entorno actual
        std::shared_ptr<Environment> enclosing;         ///< Entorno padre
    };
}

#endif // ENVIRONMENT_H
