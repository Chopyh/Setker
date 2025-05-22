#include "Tokens.h"

#include <cmath>
#include <cstdint>
#include <sstream>

// Implementación del constructor
Token::Token(TokenType type, std::string lexeme, Literal literalValue)
    : type(type), lexeme(std::move(lexeme)), literal(literalValue) {}

// Implementación de los getters
TokenType Token::getType() const {
    return type;
}

std::string Token::getLexeme() const {
    return lexeme;
}

Literal Token::getLiteral() const {
    return literal;
}

// Implementación del método print
std::string Token::print() const {
    std::stringstream ss;
    switch (type) {
        case TokenType::VAR: ss << "VAR"; break;
        case TokenType::IF: ss << "IF"; break;
        case TokenType::ELSE: ss << "ELSE"; break;
        case TokenType::WHILE: ss << "WHILE"; break;
        case TokenType::FOR: ss << "FOR"; break;
        case TokenType::FUN: ss << "FUN"; break;
        case TokenType::RETURN: ss << "RETURN"; break;
        case TokenType::AND: ss << "AND"; break;
        case TokenType::CLASS: ss << "CLASS"; break;
        case TokenType::FALSE: ss << "FALSE"; break;
        case TokenType::NIL: ss << "NIL"; break;
        case TokenType::OR: ss << "OR"; break;
        case TokenType::PRINT: ss << "PRINT"; break;
        case TokenType::SUPER: ss << "SUPER"; break;
        case TokenType::THIS: ss << "THIS"; break;
        case TokenType::TRUE: ss << "TRUE"; break;
        case TokenType::IDENTIFIER: ss << "IDENTIFIER"; break;
        case TokenType::DOT: ss << "DOT"; break;
        case TokenType::BANG: ss << "BANG"; break;
        case TokenType::BANG_EQUAL: ss << "BANG_EQUAL"; break;
        case TokenType::GREATER: ss << "GREATER"; break;
        case TokenType::GREATER_EQUAL: ss << "GREATER_EQUAL"; break;
        case TokenType::LESS: ss << "LESS"; break;
        case TokenType::LESS_EQUAL: ss << "LESS_EQUAL"; break;
        case TokenType::STRING: ss << "STRING"; break;
        case TokenType::NUMBER: ss << "NUMBER"; break;
        case TokenType::PLUS: ss << "PLUS"; break;
        case TokenType::MINUS: ss << "MINUS"; break;
        case TokenType::MULT: ss << "STAR"; break;
        case TokenType::DIV: ss << "DIV"; break;
        case TokenType::MOD: ss << "MOD"; break;
        case TokenType::EQUAL: ss << "EQUAL"; break;
        case TokenType::EQUAL_EQUAL: ss << "EQUAL_EQUAL"; break;
        case TokenType::SEMICOLON: ss << "SEMICOLON"; break;
        case TokenType::SLASH: ss << "SLASH"; break;
        case TokenType::COMMA: ss << "COMMA"; break;
        case TokenType::COLON: ss << "COLON"; break;
        case TokenType::L_PAREN: ss << "LEFT_PAREN"; break;
        case TokenType::R_PAREN: ss << "RIGHT_PAREN"; break;
        case TokenType::L_BRACE: ss << "LEFT_BRACE"; break;
        case TokenType::R_BRACE: ss << "RIGHT_BRACE"; break;
        case TokenType::L_BRACKET: ss << "LEFT_BRACKET"; break;
        case TokenType::R_BRACKET: ss << "RIGHT_BRACKET"; break;
        case TokenType::EOF_OF_FILE: ss << "EOF"; break;
    }

    std::string literalString = "null";

    if (std::holds_alternative<bool>(literal)) {
        literalString = std::get<bool>(literal) ? "true" : "false";
    } else if (std::holds_alternative<double>(literal)) {
        double value = std::get<double>(literal);
        std::ostringstream oss;
        oss.precision(15);
        if (std::abs(value - std::round(value)) < 1e-9) {
            oss << static_cast<int64_t>(std::round(value)) << ".0";
        } else {
            oss << value;
        }
        literalString = oss.str();
    } else if (std::holds_alternative<std::string>(literal)) {
        literalString = std::get<std::string>(literal);
    }

    if (type == TokenType::STRING) {
        ss << " \"" << lexeme << "\" " << literalString;
    } else if (type == TokenType::NUMBER) {
        ss << " " << lexeme << " " << literalString;
    } else if (type == TokenType::EOF_OF_FILE) {
        ss << "  " << literalString;
    } else {
        ss << " " << lexeme << " " << literalString;
    }

    return ss.str();
}