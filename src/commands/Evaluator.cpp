/**
 * @file Evaluator.cpp
 * @brief Implementación del evaluador de expresiones y ejecutor de instrucciones
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene la implementación completa del evaluador que ejecuta
 * programas escritos en Setker. Maneja la evaluación de expresiones, ejecución
 * de instrucciones, gestión de variables y funciones, y todas las operaciones
 * del lenguaje incluyendo aritmética, lógica y control de flujo.
 * 
 * Características implementadas:
 * - Evaluación de expresiones aritméticas y lógicas
 * - Manejo de variables locales y globales con scoping léxico
 * - Funciones definidas por el usuario con soporte para closures
 * - Estructuras de control (if/else, while, for)
 * - Instrucciones print y return
 * - Función nativa clock() para obtener tiempo actual
 * - Manejo robusto de errores de tiempo de ejecución
 */

#include "Evaluator.h"
#include "Parser.h"
#include "../def/Environment.h"
#include "../def/ErrorCode.h"
#include <variant>
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>

using namespace TokenTree;
using namespace Parser;

namespace Evaluator {
    // Alias para Environment para mayor claridad
    using TokenTree::Environment;

    /**
     * @brief Función auxiliar para parsear AST desde tokens
     * @param tokens Vector de tokens
     * @return std::unique_ptr<ASTNode> AST resultante
     */
    std::unique_ptr<ASTNode> parseAST(const std::pmr::vector<Token>& tokens) {
        // Usa la función interna de Parser para construir el AST
        // Requiere exponer parseAST en Parser
        return Parser::parseAST(tokens);
    }

    /**
     * @brief Determina si un valor es "verdadero" en contexto booleano
     * @param v Valor a evaluar
     * @return bool true si el valor es verdadero, false en caso contrario
     * 
     * Implementa las reglas de truthiness de Setker:
     * - nil es false
     * - false es false
     * - Todo lo demás es true
     */
    bool isTruthy(const Value& v) {
        if (std::holds_alternative<std::monostate>(v)) return false;
        if (auto b = std::get_if<bool>(&v)) return *b;
        return true;
    }

    // Declaraciones de función para evaluación con entorno
    Value evalNode(const ASTNode* node, std::shared_ptr<Environment> env);

    /**
     * @brief Evaluación con entorno global por defecto
     * @param node Nodo a evaluar
     * @return Value Resultado de la evaluación
     */
    Value evalNode(const ASTNode* node) {
        static std::shared_ptr<Environment> globalEnv = std::make_shared<Environment>();
        return evalNode(node, globalEnv);
    }

    /**
     * @brief Función principal de evaluación con entorno específico
     * @param node Nodo del AST a evaluar
     * @param env Entorno de ejecución
     * @return Value Resultado de la evaluación
     * @throws Error Para errores de tiempo de ejecución
     * @throws ReturnException Para instrucciones return
     * 
     * Esta función implementa el núcleo del intérprete, manejando todos
     * los tipos de nodos del AST y ejecutando las operaciones correspondientes.
     * Utiliza recursión para evaluar subexpresiones y mantiene el estado
     * del programa a través del entorno de variables.
     */
    Value evalNode(const ASTNode* node, std::shared_ptr<Environment> env) {
        using Type = ASTNode::Type;

        switch (node->getType()) {
            case Type::Function: {
                // Declaración de función user-defined con parámetros
                {
                    const auto& name = node->getValue();
                    const auto& children = node->getChildren();
                    // Último hijo es el cuerpo, los anteriores son parámetros
                    std::vector<std::string> params;
                    for (size_t i = 0; i + 1 < children.size(); ++i) {
                        params.push_back(children[i]->getValue());
                    }
                    const ASTNode* body = children.back().get();
                    auto func = std::make_shared<LoxFunction>(name, params, body, env);
                    env->define(node->getValue(), func);
                    return func;
                }
            }
            case Type::VarDecl: {
                Value val = std::monostate{};
                if (!node->getChildren().empty()) {
                    val = evalNode(node->getChildren()[0].get(), env);
                }
                env->define(node->getValue(), val);
                return val;
            }
            case Type::Identifier: {
                return env->get(node->getValue());
            }
            case Type::Number: {
                return std::stod(node->getValue());
            }
            case Type::Boolean: {
                return node->getValue() == "true";
            }
            case Type::Nil: {
                return std::monostate{};
            }
            case Type::String: {
                return node->getValue();
            }
            case Type::BinaryOp: {
                const auto& op = node->getValue();
                const auto& children = node->getChildren();
                // Asignación: a = b (right-associative)
                if (op == "=" && children.size() == 2) {
                    // Validar target
                    const auto* target = children[0].get();
                    if (target->getType() != ASTNode::Type::Identifier) {
                        throw Error(ErrorCodes::InvalidAssignmentTarget, "Invalid assignment target.");
                    }
                    // Evaluar valor
                    Value val = evalNode(children[1].get(), env);
                    // Asignar en entorno (lanza std::runtime_error si no existe)
                    env->assign(target->getValue(), val);
                    return val;
                }
                // Lógica OR: devuelve operand0 si truthy, sino operand1
                if (op == "or" && children.size() == 2) {
                    Value left = evalNode(children[0].get(), env);
                    if (isTruthy(left)) return left;
                    return evalNode(children[1].get(), env);
                }
                // Lógica AND: devuelve operand0 si falsy (short-circuit), sino operand1
                if (op == "and" && children.size() == 2) {
                    Value left = evalNode(children[0].get(), env);
                    if (!isTruthy(left)) return left;
                    return evalNode(children[1].get(), env);
                }
                // Handle unary operators (single child)
                if (op == "!" && children.size() == 1) {
                    Value v = evalNode(children[0].get(), env);
                    return !isTruthy(v);
                }
                if (op == "-" && children.size() == 1) {
                    Value operand = evalNode(children[0].get(), env);
                    if (!std::holds_alternative<double>(operand)) {
                        throw Error(ErrorCodes::OperandMustBeNumber, "Operand must be a number.");
                    }
                    double v = std::get<double>(operand);
                    return -v;
                }
                // Handle grouping
                if (op == "group" && children.size() == 1) {
                    return evalNode(children[0].get(), env);
                }                // Handle binary arithmetic operators
                if (children.size() == 2 && (op == "+" || op == "-" || op == "*" || op == "/" || op == "%")) {
                    Value lv = evalNode(children[0].get(), env);
                    Value rv = evalNode(children[1].get(), env);
                    if (op == "+") {
                        // String concatenation con conversión automática
                        if (auto ls = std::get_if<std::string>(&lv)) {
                            if (auto rs = std::get_if<std::string>(&rv)) {
                                return *ls + *rs;
                            } else if (auto rd = std::get_if<double>(&rv)) {
                                // String + Number
                                std::string numStr = (std::floor(*rd) == *rd) ? 
                                    std::to_string((long long)*rd) : std::to_string(*rd);
                                return *ls + numStr;
                            } else if (auto rb = std::get_if<bool>(&rv)) {
                                // String + Boolean
                                return *ls + (*rb ? "true" : "false");
                            } else if (std::holds_alternative<std::monostate>(rv)) {
                                // String + nil
                                return *ls + "nil";
                            }
                        } else if (auto ld = std::get_if<double>(&lv)) {
                            if (auto rs = std::get_if<std::string>(&rv)) {
                                // Number + String
                                std::string numStr = (std::floor(*ld) == *ld) ? 
                                    std::to_string((long long)*ld) : std::to_string(*ld);
                                return numStr + *rs;
                            }
                        } else if (auto lb = std::get_if<bool>(&lv)) {
                            if (auto rs = std::get_if<std::string>(&rv)) {
                                // Boolean + String
                                return (*lb ? "true" : "false") + *rs;
                            }
                        } else if (std::holds_alternative<std::monostate>(lv)) {
                            if (auto rs = std::get_if<std::string>(&rv)) {
                                // nil + String
                                return "nil" + *rs;
                            }
                        }
                        
                        // Si llegamos aquí con +, pero no es concatenación válida, seguir con aritmética
                    }
                    // Operaciones numéricas (-, *, /, %) requieren números
                    if (!std::holds_alternative<double>(lv) || !std::holds_alternative<double>(rv)) {
                        throw Error(ErrorCodes::OperandsMustBeNumbers, "Operands must be numbers.");
                    }
                    double left = std::get<double>(lv);
                    double right = std::get<double>(rv);
                    if (op == "+") return left + right;
                    if (op == "-") return left - right;
                    if (op == "*") return left * right;
                    if (op == "/") return left / right;
                    if (op == "%") return std::fmod(left, right);
                }
                // Handle equality
                if (children.size() == 2 && (op == "==" || op == "!=")) {
                    Value lv = evalNode(children[0].get(), env);
                    Value rv = evalNode(children[1].get(), env);
                    bool eq = false;
                    // Solo true si ambos son del mismo tipo y valor
                    if (auto ld = std::get_if<double>(&lv)) {
                        if (auto rd = std::get_if<double>(&rv)) {
                            eq = (*ld == *rd);
                        } else {
                            eq = false;
                        }
                    } else if (auto ls = std::get_if<std::string>(&lv)) {
                        if (auto rs = std::get_if<std::string>(&rv)) {
                            eq = (*ls == *rs);
                        } else {
                            eq = false;
                        }
                    } else if (auto lb = std::get_if<bool>(&lv)) {
                        if (auto rb = std::get_if<bool>(&rv)) {
                            eq = (*lb == *rb);
                        } else {
                            eq = false;
                        }
                    } else if (std::holds_alternative<std::monostate>(lv) && std::holds_alternative<std::monostate>(rv)) {
                        eq = true;
                    } else {
                        eq = false;
                    }
                    return op == "==" ? eq : !eq;
                }
                // Handle comparison
                if (children.size() == 2 && (op == "<" || op == ">" || op == "<=" || op == ">=")) {
                    Value lv = evalNode(children[0].get(), env);
                    Value rv = evalNode(children[1].get(), env);
                    // Ambos deben ser del mismo tipo y deben ser números
                    if (lv.index() != rv.index() || !std::holds_alternative<double>(lv)) {
                        throw Error(ErrorCodes::OperandsMustBeNumbers, "Operands must be numbers.");
                    }
                    double left = std::get<double>(lv);
                    double right = std::get<double>(rv);
                    if (op == "<") return left < right;
                    if (op == ">") return left > right;
                    if (op == "<=") return left <= right;
                    if (op == ">=") return left >= right;
                }
                break;
            }
            case Type::Program: {
                // Ejecutar todos los hijos (statements), con entorno local si es bloque
                bool isBlock = (node->getValue() == "block");
                // Si es un bloque, crear entorno anidado que hereda de env, sino usar env
                auto execEnv = isBlock ? std::make_shared<Environment>(env) : env;
                Value last = std::monostate{};
                const auto& children = node->getChildren();
                for (size_t i = 0; i < children.size(); ++i) {
                    try {
                        last = evalNode(children[i].get(), execEnv);
                    } catch (const Error& e) {
                        throw Error(e.type, e.message + "\n[line " + std::to_string(i + 1) + "]");
                    } catch (const std::runtime_error& e) {
                        throw Error(ErrorCodes::RuntimeError, std::string(e.what()) + "\n[line " + std::to_string(i + 1) + "]");
                    }
                }
                return last;
            }
            case Type::PrintStmt: {
                // Evaluar la expresión hija y mostrarla
                if (!node->getChildren().empty()) {
                    Value value = evalNode(node->getChildren()[0].get(), env);
                    if (std::holds_alternative<std::monostate>(value)) {
                        std::cout << "nil";
                    } else if (auto b = std::get_if<bool>(&value)) {
                        std::cout << (*b ? "true" : "false");
                    } else if (auto d = std::get_if<double>(&value)) {
                        if (std::floor(*d) == *d) std::cout << (long long)*d;
                        else std::cout << *d;
                    }
                    // Funciones definidas
                    else if (auto f = std::get_if<FunctionPtr>(&value)) {
                        std::cout << "<fn " << (*f)->name << ">";
                    }
                    else if (auto s = std::get_if<std::string>(&value)) {
                        std::cout << *s;
                    }
                    std::cout << std::endl;
                }
                return std::monostate{};
            }
            case Type::IfStmt: {
                // Evaluar condición y ejecutar rama then o else
                Value cond = evalNode(node->getChildren()[0].get(), env);
                if (isTruthy(cond)) {
                    return evalNode(node->getChildren()[1].get(), env);
                }
                // Ejecutar rama else si existe
                if (node->getChildren().size() == 3) {
                    return evalNode(node->getChildren()[2].get(), env);
                }
                return std::monostate{};
            }
            case Type::WhileStmt: {
                // Ejecutar bucle while: evaluar condición y ejecutar cuerpo mientras sea truthy
                while (true) {
                    Value cond = evalNode(node->getChildren()[0].get(), env);
                    if (!isTruthy(cond)) break;
                    evalNode(node->getChildren()[1].get(), env);
                }
                return std::monostate{};
            }
            case Type::ReturnStmt: {
                // Devolver valor desde una función
                Value retVal = std::monostate{};
                if (!node->getChildren().empty()) {
                    retVal = evalNode(node->getChildren()[0].get(), env);
                }
                throw ReturnException(retVal);
            }
            case Type::Call: {
                const auto& name = node->getValue();
                // Función nativa 'clock'
                if (name == "clock") {
                    double t = static_cast<double>(std::time(nullptr));
                    return t;
                }
                // Obtener función definida o variable
                Value callee = env->get(name);
                // Función definida por usuario con parámetros
                if (auto funcPtr = std::get_if<FunctionPtr>(&callee)) {
                    auto function = *funcPtr;
                    const auto& args = node->getChildren();
                    // Verificar número de argumentos
                    if (args.size() != function->params.size()) {
                        throw Error(ErrorCodes::ArgumentCountMismatch, "Expected " + std::to_string(function->params.size()) + " args but got " + std::to_string(args.size()) + ".");
                    }
                    // Crear nuevo entorno con closure de la función
                    auto localEnv = std::make_shared<Environment>(function->closure);
                    // Evaluar y definir parámetros
                    for (size_t i = 0; i < args.size(); ++i) {
                        Value val = evalNode(args[i].get(), env);
                        localEnv->define(function->params[i], val);
                    }
                    // Ejecutar cuerpo de función capturando return
                    try {
                        evalNode(function->body, localEnv);
                        return std::monostate{};
                    } catch (const ReturnException& ret) {
                        return ret.value;
                    }
                }
                // No es una función
                throw Error(ErrorCodes::CallOnNonFunction, "Attempt to call non-function '" + name + "'.");
            }
            default:
                break;
        }
        return std::monostate{};
    }

    int evaluate(const std::pmr::vector<Token>& tokens) {
        try {
            auto ast = parseAST(tokens);
            Value result = evalNode(ast.get());
            if (std::holds_alternative<std::monostate>(result)) {
                std::cout << "nil";
            } else if (auto b = std::get_if<bool>(&result)) {
                std::cout << (*b ? "true" : "false");
            } else if (auto d = std::get_if<double>(&result)) {
                // imprimir sin .0 si es entero
                if (std::floor(*d) == *d) std::cout << (long long)*d;
                else std::cout << *d;
            } else if (auto s = std::get_if<std::string>(&result)) {
                std::cout << *s;
            }
            std::cout << std::endl;
            return 0;
        } catch (const Error& e) {
            std::cerr << e.message;
            return e.type.code;
        }
    }
} // namespace Evaluator