/**
 * @file ASTNode.h
 * @brief Definición del Árbol de Sintaxis Abstracta (AST) para el parser
 * @author Javier
 * @date 2025
 * 
 * Este archivo define la estructura de nodos que conforman el Árbol de Sintaxis
 * Abstracta utilizado por el parser para representar la estructura jerárquica
 * del código fuente analizado.
 */

#ifndef ASTNODE_H
#define ASTNODE_H

#include <memory>
#include <string>
#include <vector>
#include <memory_resource>

namespace TokenTree {
    /**
     * @class ASTNode
     * @brief Nodo del Árbol de Sintaxis Abstracta
     * 
     * Representa un nodo individual en el AST. Cada nodo tiene un tipo específico,
     * un valor asociado y puede tener nodos hijos. Esta estructura permite
     * representar la jerarquía completa del programa analizado.
     */
    class ASTNode {
    public:
        /**
         * @enum Type
         * @brief Enumeración de todos los tipos de nodos del AST
         * 
         * Define los diferentes tipos de construcciones del lenguaje que
         * pueden ser representadas como nodos en el AST.
         */
        enum class Type { 
            Number,      ///< Literal numérico
            BinaryOp,    ///< Operación binaria (+, -, *, /, etc.)
            String,      ///< Literal de cadena
            Boolean,     ///< Literal booleano (true/false)
            Nil,         ///< Literal nil
            PrintStmt,   ///< Instrucción print
            IfStmt,      ///< Instrucción condicional if
            WhileStmt,   ///< Bucle while
            ReturnStmt,  ///< Instrucción return
            Function,    ///< Declaración de función
            Call,        ///< Llamada a función
            Program,     ///< Programa o bloque de código
            VarDecl,     ///< Declaración de variable
            Identifier   ///< Identificador (nombre de variable/función)
        };
        
    private:
        Type type;                                          ///< Tipo del nodo
        std::string value;                                  ///< Valor asociado al nodo
        std::vector<std::unique_ptr<ASTNode>> children;    ///< Nodos hijos
        
    public:
        /**
         * @brief Constructor de ASTNode
         * @param type Tipo del nodo
         * @param value Valor asociado al nodo
         */
        ASTNode(Type type, std::string value);
        
        /**
         * @brief Agrega un nodo hijo al nodo actual
         * @param child Puntero único al nodo hijo a agregar
         */
        void addChild(std::unique_ptr<ASTNode> child);
        
        /**
         * @brief Convierte el nodo y sus hijos a representación textual
         * @return std::string Representación en cadena del AST
         * 
         * Genera una representación textual del nodo y todos sus descendientes,
         * útil para debugging y visualización de la estructura del AST.
         */
        std::string toString() const;
        
        /**
         * @brief Obtiene el tipo del nodo
         * @return Type Tipo del nodo
         */
        Type getType() const;
        
        /**
         * @brief Obtiene el valor asociado al nodo
         * @return const std::string& Referencia al valor del nodo
         */
        const std::string& getValue() const;
        
        /**
         * @brief Obtiene los nodos hijos
         * @return const std::vector<std::unique_ptr<ASTNode>>& Referencia al vector de hijos
         */
        const std::vector<std::unique_ptr<ASTNode>>& getChildren() const;
    };
}

#endif // ASTNODE_H
