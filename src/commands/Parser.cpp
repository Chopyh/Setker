#include "Parser.h"
#include <iostream>

namespace Parser {
    void printAST(const ASTNode& node, int depth) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << node.type << ": " << node.value << std::endl;
        for (const auto& child : node.children) {
            printAST(child, depth + 1);
        }
    }

    ASTNode parsePrimary(const std::vector<Token>& tokens, size_t& index) {
        if (index >= tokens.size()) return {"EOF", "", {}};

        const auto& token = tokens[index++];
        if (token.getType() == TokenType::NUMBER || token.getType() == TokenType::STRING) {
            return {"Literal", token.getLiteral(), {}};
        } else if (token.getType() == TokenType::L_PAREN) {
            ASTNode expr = parseExpression(tokens, index);
            if (index < tokens.size() && tokens[index].getType() == TokenType::R_PAREN) {
                index++;
            } else {
                std::cerr << "Error: Missing closing parenthesis." << std::endl;
            }
            return expr;
        }

        std::cerr << "Error: Unexpected token: " << token.getLiteral() << std::endl;
        return {"Error", token.getLiteral(), {}};
    }

    ASTNode parseTerm(const std::vector<Token>& tokens, size_t& index) {
        ASTNode node = parsePrimary(tokens, index);

        while (index < tokens.size() && (tokens[index].getType() == TokenType::MULT || tokens[index].getType() == TokenType::SLASH)) {
            const auto& op = tokens[index++];
            ASTNode right = parsePrimary(tokens, index);
            node = {"BinaryOp", op.getLiteral(), {node, right}};
        }

        return node;
    }

    ASTNode parseExpression(const std::vector<Token>& tokens, size_t& index) {
        ASTNode node = parseTerm(tokens, index);

        while (index < tokens.size() && (tokens[index].getType() == TokenType::PLUS || tokens[index].getType() == TokenType::MINUS)) {
            const auto& op = tokens[index++];
            ASTNode right = parseTerm(tokens, index);
            node = {"BinaryOp", op.getLiteral(), {node, right}};
        }

        return node;
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