/**
 * @file Tokens.h
 * @brief Definiciones de tokens y tipos de datos para el analizador léxico
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene las definiciones de todos los tipos de tokens
 * reconocidos por el analizador léxico de Setker, así como la clase Token
 * que encapsula la información de cada token identificado.
 */

#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <variant>

/**
 * @enum TokenType
 * @brief Enumeración de todos los tipos de tokens reconocidos por el lenguaje
 * 
 * Define todos los tipos de tokens que el analizador léxico puede identificar,
 * organizados en categorías:
 * - Palabras clave del lenguaje
 * - Identificadores y literales
 * - Operadores aritméticos, lógicos y de comparación
 * - Delimitadores y símbolos especiales
 * - Marcador de fin de archivo
 */
enum class TokenType {
     // Palabras clave (keywords)
     VAR,        ///< Declaración de variable: 'var'
     IF,         ///< Condicional: 'if'
     ELSE,       ///< Alternativa: 'else'
     WHILE,      ///< Bucle: 'while'
     FOR,        ///< Bucle for: 'for'
     FUN,        ///< Declaración de función: 'fun'
     RETURN,     ///< Retorno de función: 'return'
     AND,        ///< Operador lógico AND: 'and'
     CLASS,      ///< Declaración de clase: 'class'
     FALSE,      ///< Literal booleano: 'false'
     NIL,        ///< Valor nulo: 'nil'
     OR,         ///< Operador lógico OR: 'or'
     PRINT,      ///< Instrucción de impresión: 'print'
     SUPER,      ///< Referencia a superclase: 'super'
     THIS,       ///< Referencia a instancia actual: 'this'
     TRUE,       ///< Literal booleano: 'true'
     
     // Identificadores y literales
     IDENTIFIER, ///< Nombre de variable, función o clase
     STRING,     ///< Literal de cadena: "texto"
     NUMBER,     ///< Literal numérico: 123.45
     
     // Operadores
     PLUS,           ///< Suma: '+'
     MINUS,          ///< Resta: '-'
     MULT,           ///< Multiplicación: '*'
     DIV,            ///< División: '/'
     MOD,            ///< Módulo: '%'
     EQUAL,          ///< Asignación: '='
     DOT,            ///< Punto: '.'
     EQUAL_EQUAL,    ///< Igualdad: '=='
     BANG,           ///< Negación lógica: '!
     BANG_EQUAL,     ///< Desigualdad: '!='
     GREATER,        ///< Mayor que: '>'
     GREATER_EQUAL,  ///< Mayor o igual: '>='
     LESS,           ///< Menor que: '<'
     LESS_EQUAL,     ///< Menor o igual: '<='
     
     // Delimitadores y símbolos
     SEMICOLON,      ///< Punto y coma: ';'
     COMMA,          ///< Coma: ','
     COLON,          ///< Dos puntos: ':'
     L_PAREN,        ///< Paréntesis izquierdo: '('
     R_PAREN,        ///< Paréntesis derecho: ')'
     L_BRACE,        ///< Llave izquierda: '{'
     R_BRACE,        ///< Llave derecha: '}'
     L_BRACKET,      ///< Corchete izquierdo: '['
     R_BRACKET,      ///< Corchete derecho: ']'
     SLASH,          ///< Barra diagonal: '/'
     
     // Fin de archivo
     EOF_OF_FILE     ///< Marcador de fin de archivo
 };

/**
 * @typedef Literal
 * @brief Tipo variante para representar valores literales de tokens
 * 
 * Utiliza std::variant para almacenar diferentes tipos de valores:
 * - void*: Para tokens sin valor (por ejemplo, operadores)
 * - bool: Para literales booleanos (true/false)
 * - double: Para literales numéricos
 * - std::string: Para literales de cadena
 */
using Literal = std::variant<void*, bool, double, std::string>;

/**
 * @class Token
 * @brief Representa un token individual identificado por el analizador léxico
 * 
 * Encapsula toda la información necesaria sobre un token:
 * - Su tipo (TokenType)
 * - Su representación textual original (lexeme)
 * - Su valor literal si aplicable
 */
class Token {
    TokenType type;     ///< Tipo del token
    std::string lexeme; ///< Texto original del token
    Literal literal;    ///< Valor literal del token

public:
    /**
     * @brief Constructor completo de Token
     * @param type Tipo del token
     * @param lexeme Texto original del token
     * @param literalValue Valor literal del token
     */
    Token(TokenType type, std::string lexeme, Literal literalValue);
    
    /**
     * @brief Constructor simplificado para tokens sin valor literal
     * @param type Tipo del token
     * @param lexeme Texto original del token
     */
    Token(TokenType type, std::string lexeme) : Token(type, lexeme, nullptr) {}

    /**
     * @brief Obtiene el tipo del token
     * @return TokenType Tipo del token
     */
    [[nodiscard]] TokenType getType() const;
    
    /**
     * @brief Obtiene el texto original del token
     * @return std::string Lexema del token
     */
    [[nodiscard]] std::string getLexeme() const;
    
    /**
     * @brief Obtiene el valor literal del token
     * @return Literal Valor literal del token
     */
    [[nodiscard]] Literal getLiteral() const;
    
    /**
     * @brief Genera una representación en cadena del token para debugging
     * @return std::string Representación textual del token
     */
    std::string print() const;
};

#endif // TOKENS_H