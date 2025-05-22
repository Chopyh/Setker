#include "Keywords.h"

static constexpr std::string_view AND = "and";
static constexpr std::string_view CLASS = "class";
static constexpr std::string_view FALSE = "false";
static constexpr std::string_view NIL = "nil";
static constexpr std::string_view OR = "or";
static constexpr std::string_view PRINT = "print";
static constexpr std::string_view SUPER = "super";
static constexpr std::string_view THIS = "this";
static constexpr std::string_view TRUE = "true";
static constexpr std::string_view FUN = "fun";

TokenType Keywords::valorateKeyword(std::string keyword) {
    if (keyword == AND) return TokenType::AND;
    if (keyword == CLASS) return TokenType::CLASS;
    if (keyword == ELSE) return TokenType::ELSE;
    if (keyword == FALSE) return TokenType::FALSE;
    if (keyword == FOR) return TokenType::FOR;
    if (keyword == FUN) return TokenType::FUN;
    if (keyword == IF) return TokenType::IF;
    if (keyword == NIL) return TokenType::NIL;
    if (keyword == OR) return TokenType::OR;
    if (keyword == PRINT) return TokenType::PRINT;
    if (keyword == RETURN) return TokenType::RETURN;
    if (keyword == SUPER) return TokenType::SUPER;
    if (keyword == THIS) return TokenType::THIS;
    if (keyword == TRUE) return TokenType::TRUE;
    if (keyword == VAR) return TokenType::VAR;
    if (keyword == WHILE) return TokenType::WHILE;
    return TokenType::IDENTIFIER;
}
