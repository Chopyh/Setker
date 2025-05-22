#ifndef PARSER_H
#define PARSER_H

#include <memory_resource>
#include "../def/Tokens.h"
#include "../def/ASTNode.h"
#include "../def/ErrorCode.h"

using namespace TokenTree;
using Error = TokenTree::Error;

namespace Parser {
    int parse(const std::pmr::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parseAST(const std::pmr::vector<Token>& tokens);
}

#endif // PARSER_H
