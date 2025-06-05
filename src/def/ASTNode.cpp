/**
 * @file ASTNode.cpp
 * @brief Implementación de la clase ASTNode y sus métodos
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene la implementación de todos los métodos de la clase ASTNode,
 * incluyendo la lógica para la representación textual del AST y manejo de nodos hijos.
 */

#include "ASTNode.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace TokenTree {

ASTNode::ASTNode(Type type, std::string value)
    : type(type), value(std::move(value)) {}

void ASTNode::addChild(std::unique_ptr<ASTNode> child) {
    children.emplace_back(std::move(child));
}

std::string ASTNode::toString() const {
    switch (type) {
        case Type::Number: {
            std::string s = value;
            if (s == "true" || s == "false") return s;
            try {
                std::stod(s);
                auto dotPos = s.find('.');
                if (dotPos == std::string::npos) return s + ".0";
                while (!s.empty() && s.back() == '0') s.pop_back();
                if (!s.empty() && s.back() == '.') s.push_back('0');
                return s;
            } catch (...) {
                return s;
            }
        }
        case Type::Boolean:
        case Type::String:
        case Type::Nil:
            return value;
        case Type::BinaryOp: {
            std::string res = "(" + value;
            for (const auto& child : children) {
                res += " " + child->toString();
            }
            res += ")";
            return res;
        }
        case Type::PrintStmt:
            return "(print " + (children.empty() ? "" : children[0]->toString()) + ")";
        case Type::Program: {
            std::string res = "(program";
            for (const auto& child : children) {
                res += " " + child->toString();
            }
            res += ")";
            return res;
        }
        case Type::VarDecl: {
            // value = nombre variable, children[0] = expresión inicial
            std::string res = "(var " + value;
            if (!children.empty()) res += " = " + children[0]->toString();
            res += ")";
            return res;
        }
        case Type::Identifier:
            return value;
        default:
            return "";
    }
}

ASTNode::Type ASTNode::getType() const {
    return type;
}

const std::string& ASTNode::getValue() const {
    return value;

}

const std::vector<std::unique_ptr<ASTNode>>& ASTNode::getChildren() const {
    return children;
}

} // namespace TokenTree
