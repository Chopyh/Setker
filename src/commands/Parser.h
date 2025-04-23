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
    void printAST(const ASTNode& node, int depth = 0);
    ASTNode parsePrimary(const std::vector<Token>& tokens, size_t& index);
    ASTNode parseTerm(const std::vector<Token>& tokens, size_t& index);
    ASTNode parseExpression(const std::vector<Token>& tokens, size_t& index);
    int parse(const std::vector<Token>& tokens);
}

#endif // PARSER_H