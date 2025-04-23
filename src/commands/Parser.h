#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../def/Tokens.h"

namespace Parser {
    struct ASTNode {
        std::string type;
        std::string value;
        std::vector<ASTNode> children;
    };

    int parse(const std::vector<Token>& tokens);
}

#endif // PARSER_H