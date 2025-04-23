#ifndef KEWYORDS_H
#define KEWYORDS_H

#include <string_view>
#include "Tokens.h"

class Keywords {
public:
    static constexpr std::string_view VAR = "var";
    static constexpr std::string_view IF = "if";
    static constexpr std::string_view ELSE = "else";
    static constexpr std::string_view WHILE = "while";
    static constexpr std::string_view FOR = "for";
    static constexpr std::string_view FUNCTION = "fn";
    static constexpr std::string_view RETURN = "return";

    static TokenType valorateKeyword(std::string keyword);
};
#endif //KEWYORDS_H