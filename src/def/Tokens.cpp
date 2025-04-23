#include "Tokens.h"
#include <iostream>

// Implementación del constructor
Token::Token(TokenType type, std::string lexeme, std::string literalValue)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literalValue)) {}

// Implementación de los getters
TokenType Token::getType() const {
    return type;
}

std::string Token::getLexeme() const {
    return lexeme;
}

std::string Token::getLiteral() const {
    return literal;
}

// Implementación del método print
void Token::print() const {
    switch (type) {
        case TokenType::VAR: std::cout << "VAR"; break;
        case TokenType::IF: std::cout << "IF"; break;
        case TokenType::ELSE: std::cout << "ELSE"; break;
        case TokenType::WHILE: std::cout << "WHILE"; break;
        case TokenType::FOR: std::cout << "FOR"; break;
        case TokenType::FUNCTION: std::cout << "FUNCTION"; break;
        case TokenType::RETURN: std::cout << "RETURN"; break;
        case TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
        case TokenType::DOT: std::cout << "DOT"; break;
        case TokenType::BANG: std::cout << "BANG"; break;
        case TokenType::BANG_EQUAL: std::cout << "BANG_EQUAL"; break;
        case TokenType::GREATER: std::cout << "GREATER"; break;
        case TokenType::GREATER_EQUAL: std::cout << "GREATER_EQUAL"; break;
        case TokenType::LESS: std::cout << "LESS"; break;
        case TokenType::LESS_EQUAL: std::cout << "LESS_EQUAL"; break;
        case TokenType::STRING: std::cout << "STRING"; break;
        case TokenType::NUMBER: std::cout << "NUMBER"; break;
        case TokenType::PLUS: std::cout << "PLUS"; break;
        case TokenType::MINUS: std::cout << "MINUS"; break;
        case TokenType::MULT: std::cout << "STAR"; break;
        case TokenType::DIV: std::cout << "DIV"; break;
        case TokenType::MOD: std::cout << "MOD"; break;
        case TokenType::ASSIGN: std::cout << "ASSIGN"; break;
        case TokenType::EQUAL: std::cout << "EQUAL"; break;
        case TokenType::EQUAL_EQUAL: std::cout << "EQUAL_EQUAL"; break;
        case TokenType::SEMICOLON: std::cout << "SEMICOLON"; break;
        case TokenType::SLASH: std::cout << "SLASH"; break;
        case TokenType::COMMA: std::cout << "COMMA"; break;
        case TokenType::COLON: std::cout << "COLON"; break;
        case TokenType::L_PAREN: std::cout << "LEFT_PAREN"; break;
        case TokenType::R_PAREN: std::cout << "RIGHT_PAREN"; break;
        case TokenType::L_BRACE: std::cout << "LEFT_BRACE"; break;
        case TokenType::R_BRACE: std::cout << "RIGHT_BRACE"; break;
        case TokenType::L_BRACKET: std::cout << "LEFT_BRACKET"; break;
        case TokenType::R_BRACKET: std::cout << "RIGHT_BRACKET"; break;
        case TokenType::EOF_OF_FILE: std::cout << "EOF"; break;
    }
    std::cout << " " << getLexeme();

    if (!literal.empty()) {
        std::cout << " " << getLiteral();
    } else {
        std::cout << " null" << std::endl;
    }
}