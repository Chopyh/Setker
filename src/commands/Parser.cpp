#include "Parser.h"
#include <iostream>

namespace Parser {
    void printAST(const ASTNode& node, int depth = 0) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << node.type << ": " << node.value << std::endl;
        for (const auto& child : node.children) {
            printAST(child, depth + 1);
        }
    }

    ASTNode parseExpression(const std::vector<Token>& tokens, size_t& index) {
        // Por ahora, simplemente crea un nodo con el token actual
        if (index >= tokens.size()) return {"EOF", "", {}};

        const auto& token = tokens[index++];
        return {"Expression", token.getLiteral(), {}};
    }

    int parse(const std::vector<Token>& tokens) {
        size_t index = 0;
        ASTNode root = {"Program", "", {}};

        while (index < tokens.size()) {
            root.children.push_back(parseExpression(tokens, index));
        }

        printAST(root);
        return 0;
    }
}