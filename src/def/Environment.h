#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include "ASTNode.h"

namespace Evaluator { struct LoxFunction; using FunctionPtr = std::shared_ptr<LoxFunction>; }

namespace TokenTree {
    class Environment {
    public:
        using Value = std::variant<std::monostate, double, bool, std::string, Evaluator::FunctionPtr>;
        Environment();
        Environment(std::shared_ptr<Environment> enclosing);
        void define(const std::string& name, const Value& value);
        Value get(const std::string& name) const;
        void assign(const std::string& name, const Value& value);
    private:
        std::unordered_map<std::string, Value> values;
        std::shared_ptr<Environment> enclosing;
    };
}

#endif // ENVIRONMENT_H
