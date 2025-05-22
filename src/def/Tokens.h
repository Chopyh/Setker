#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <variant>

enum class TokenType {
     // Palabras clave (keywords)
     VAR, IF, ELSE, WHILE, FOR, FUN, RETURN, AND, CLASS, FALSE, NIL, OR, PRINT, SUPER, THIS, TRUE,
     // Identificadores y literales
     IDENTIFIER, STRING, NUMBER,
     // Operadores
     PLUS, MINUS, MULT, DIV, MOD, EQUAL, DOT, EQUAL_EQUAL, BANG, BANG_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
     // Delimitadores y símbolos
     SEMICOLON, COMMA, COLON, L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET, SLASH,
     // Fin de archivo
     EOF_OF_FILE
 };

using Literal = std::variant<void*, bool, double, std::string>;

class Token {
    TokenType type;
    std::string lexeme;
    Literal literal;

public:
    Token(TokenType type, std::string lexeme, Literal literalValue);
    Token(TokenType type, std::string lexeme) : Token(type, lexeme, nullptr) {}

    [[nodiscard]] TokenType getType() const;
    [[nodiscard]] std::string getLexeme() const;
    [[nodiscard]] Literal getLiteral() const;
    std::string print() const;
};

#endif // TOKENS_H