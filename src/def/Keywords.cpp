#include "Keywords.h"

constexpr std::string_view Keywords::VAR;
constexpr std::string_view Keywords::IF;
constexpr std::string_view Keywords::ELSE;
constexpr std::string_view Keywords::WHILE;
constexpr std::string_view Keywords::FOR;
constexpr std::string_view Keywords::FUNCTION;
constexpr std::string_view Keywords::RETURN;

TokenType Keywords::valorateKeyword(std::string keyword) {
    if (keyword == VAR) {
        return TokenType::VAR;
    }
    if (keyword == IF) {
        return TokenType::IF;
    }
    if (keyword == ELSE) {
        return TokenType::ELSE;
    }
    if (keyword == WHILE) {
        return TokenType::WHILE;
    }
    if (keyword == FOR) {
        return TokenType::FOR;
    }
    if (keyword == FUNCTION) {
        return TokenType::FUNCTION;
    }
    if (keyword == RETURN) {
        return TokenType::RETURN;
    }

    return TokenType::IDENTIFIER;
}
