#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <memory_resource>
#include <vector>
#include "../def/Tokens.h"
#include "../def/ASTNode.h"
#include "../def/Environment.h"
#include "../def/ErrorCode.h"

namespace Evaluator {
    using Error = TokenTree::Error;
    // Alias de tipo de valor para nodos y retorno
    using Value = ::TokenTree::Environment::Value;

    // Funciones definidas por el usuario con parámetros
    struct LoxFunction { 
        std::string name; 
        std::vector<std::string> params;
        const TokenTree::ASTNode* body; 
        std::shared_ptr<TokenTree::Environment> closure; 
        LoxFunction(std::string name, std::vector<std::string> params, const TokenTree::ASTNode* body, std::shared_ptr<TokenTree::Environment> closure) 
            : name(std::move(name)), params(std::move(params)), body(body), closure(closure) {} 
    };
    // Excepción interna para devolver valores de función
    struct ReturnException {
        // Valor retornado de la función
        Value value;
        explicit ReturnException(const Value& v)
            : value(v) {}
    };
    // Evalúa expresiones; en esta etapa solo maneja literales true, false y nil
    int evaluate(const std::pmr::vector<Token>& tokens);
    // Evalúa un ASTNode directamente (para soporte de 'run')
    using FunctionPtr = std::shared_ptr<LoxFunction>;
    Value evalNode(const TokenTree::ASTNode* node);
}

#endif // EVALUATOR_H
