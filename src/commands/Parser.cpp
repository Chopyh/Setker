#include "Parser.h"
#include "../def/ErrorCode.h"
#include <iostream>
#include <variant>
#include <sstream>
#include <stdexcept>

namespace Parser {
    // Declaraciones adelantadas para niveles de precedencia
    static std::unique_ptr<ASTNode> parseEquality(const std::pmr::vector<Token>& tokens, size_t& pos);
    static std::unique_ptr<ASTNode> parseComparison(const std::pmr::vector<Token>& tokens, size_t& pos);
    static std::unique_ptr<ASTNode> parseAdditive(const std::pmr::vector<Token>& tokens, size_t& pos);
    static std::unique_ptr<ASTNode> parseExpression(const std::pmr::vector<Token>& tokens, size_t& pos);
    // Nuevo: lógica AND (izquierda asociativa)
    static std::unique_ptr<ASTNode> parseAnd(const std::pmr::vector<Token>& tokens, size_t& pos);
    // Lógica OR (izquierda asociativa)
    static std::unique_ptr<ASTNode> parseOr(const std::pmr::vector<Token>& tokens, size_t& pos);
    // Asignación (right-associative)
    static std::unique_ptr<ASTNode> parseAssignment(const std::pmr::vector<Token>& tokens, size_t& pos);
    static std::unique_ptr<ASTNode> parseStatement(const std::pmr::vector<Token>& tokens, size_t& pos);
    static std::unique_ptr<ASTNode> parsePrimary(const std::pmr::vector<Token>& tokens, size_t& pos);
    // Llamadas de función: postfijo a primary
    static std::unique_ptr<ASTNode> parseCall(const std::pmr::vector<Token>& tokens, size_t& pos);

    // Función para parsear operadores unarios (! y -)
    static std::unique_ptr<ASTNode> parseUnary(const std::pmr::vector<Token>& tokens, size_t& pos) {
        if (pos < tokens.size()) {
            auto type = tokens[pos].getType();
            if (type == TokenType::BANG || type == TokenType::MINUS) {
                std::string op = tokens[pos].getLexeme();
                pos++; // consumir operador
                auto right = parseUnary(tokens, pos);
                auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
                node->addChild(std::move(right));
                return node;
            }
        }
        return parseCall(tokens, pos);
    }

    // Agrupación, literales y variables en parsePrimary
    static std::unique_ptr<ASTNode> parsePrimary(const std::pmr::vector<Token>& tokens, size_t& pos) {
        const Token& token = tokens[pos];
        
        // Error si falta expresión antes de ')' o fin
        if (token.getType() == TokenType::R_PAREN || token.getType() == TokenType::EOF_OF_FILE) {
            throw Error(ErrorCodes::ParseError, "Error at '" + token.getLexeme() + "': Expect expression.\n");
        }

        // Agrupación con paréntesis
        if (token.getType() == TokenType::L_PAREN) {
            pos++; // consumir '('
            auto inner = parseExpression(tokens, pos);
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                if (pos < tokens.size() && tokens[pos].getType() == TokenType::EOF_OF_FILE) {
                    throw Error(ErrorCodes::ParseError, "Error at end: Expect ')'\n");
                } else {
                    throw Error(ErrorCodes::ParseError, "Expected ')'\n");
                }
            }
            pos++; // consumir ')'
            // crear nodo de agrupación
            auto groupNode = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, "group");
            groupNode->addChild(std::move(inner));
            return groupNode;
        }

        // Literales de cadena
        if (token.getType() == TokenType::STRING) {
            std::string literal = std::get<std::string>(token.getLiteral());
            pos++;
            return std::make_unique<ASTNode>(ASTNode::Type::String, literal);
        }
        // Literales booleanos
        if (token.getType() == TokenType::TRUE || token.getType() == TokenType::FALSE) {
            std::string lexeme = token.getLexeme();
            pos++;
            return std::make_unique<ASTNode>(ASTNode::Type::Boolean, lexeme);
        }
        // Literal nil
        if (token.getType() == TokenType::NIL) {
            std::string lexeme = token.getLexeme();
            pos++;
            return std::make_unique<ASTNode>(ASTNode::Type::Nil, lexeme);
        }
        // Literales numéricos
        if (token.getType() == TokenType::NUMBER) {
            std::string lexeme = token.getLexeme();
            pos++;
            return std::make_unique<ASTNode>(ASTNode::Type::Number, lexeme);
        }
        // Identificadores
        if (token.getType() == TokenType::IDENTIFIER) {
            std::string name = token.getLexeme();
            pos++;
            return std::make_unique<ASTNode>(ASTNode::Type::Identifier, name);
        }
        // Token inesperado
        throw Error(ErrorCodes::ParseError, "Error at '" + token.getLexeme() + "': Expect expression.\n");
    }

    // Implementa llamada nativa: primary ( '(' [args] ')' )* 
    static std::unique_ptr<ASTNode> parseCall(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto expr = parsePrimary(tokens, pos);
        while (pos < tokens.size() && tokens[pos].getType() == TokenType::L_PAREN) {
            pos++; // consumir '('
            // parsear lista de argumentos
            std::vector<std::unique_ptr<ASTNode>> args;
            if (pos < tokens.size() && tokens[pos].getType() != TokenType::R_PAREN) {
                do {
                    args.push_back(parseExpression(tokens, pos));
                    if (pos < tokens.size() && tokens[pos].getType() == TokenType::COMMA) {
                        pos++; // consumir ',' y seguir
                    } else {
                        break;
                    }
                } while (true);
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ')' after arguments.\n");
            }
            pos++; // consumir ')'
            // crear nodo de llamada con nombre y argumentos
            auto callNode = std::make_unique<ASTNode>(ASTNode::Type::Call, expr->getValue());
            for (auto &arg : args) callNode->addChild(std::move(arg));
            expr = std::move(callNode);
        }
        return expr;
    }

    // Implementación de asignación: IDENTIFIER '=' assignment
    static std::unique_ptr<ASTNode> parseAssignment(const std::pmr::vector<Token>& tokens, size_t& pos) {
        // Parse OR expressions first
        auto expr = parseOr(tokens, pos);
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::EQUAL) {
            pos++; // consumir '='
            auto value = parseAssignment(tokens, pos);
            // validación de target
            if (expr->getType() != ASTNode::Type::Identifier) {
                throw Error(ErrorCodes::InvalidAssignmentTarget);
            }
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, "=");
            node->addChild(std::move(expr));
            node->addChild(std::move(value));
            return node;
        }
        return expr;
    }

    // Implementación de OR: and ( 'or' and )*
    static std::unique_ptr<ASTNode> parseOr(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseAnd(tokens, pos);
        while (pos < tokens.size() && tokens[pos].getType() == TokenType::OR) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir 'or'
            auto right = parseAnd(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Implementación de AND: equality ( 'and' equality )*
    static std::unique_ptr<ASTNode> parseAnd(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseEquality(tokens, pos);
        while (pos < tokens.size() && tokens[pos].getType() == TokenType::AND) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir 'and'
            auto right = parseEquality(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Función para parsear operaciones multiplicativas (*, /, %)
    static std::unique_ptr<ASTNode> parseMultiplicative(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseUnary(tokens, pos);
        while (pos < tokens.size() &&
               (tokens[pos].getType() == TokenType::MULT || tokens[pos].getType() == TokenType::SLASH || tokens[pos].getType() == TokenType::MOD)) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir '*', '/', o '%'
            auto right = parseUnary(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Función para parsear operaciones aditivas (+, -)
    static std::unique_ptr<ASTNode> parseAdditive(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseMultiplicative(tokens, pos);
        while (pos < tokens.size() &&
               (tokens[pos].getType() == TokenType::PLUS || tokens[pos].getType() == TokenType::MINUS)) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir '+' o '-'
            auto right = parseMultiplicative(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Función para parsear comparaciones (<, <=, >, >=)
    static std::unique_ptr<ASTNode> parseComparison(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseAdditive(tokens, pos);
        while (pos < tokens.size() &&
               (tokens[pos].getType() == TokenType::LESS || tokens[pos].getType() == TokenType::LESS_EQUAL ||
                tokens[pos].getType() == TokenType::GREATER || tokens[pos].getType() == TokenType::GREATER_EQUAL)) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir operador de comparación
            auto right = parseAdditive(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Función para parsear igualdad (==, !=)
    static std::unique_ptr<ASTNode> parseEquality(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto left = parseComparison(tokens, pos);
        while (pos < tokens.size() &&
               (tokens[pos].getType() == TokenType::EQUAL_EQUAL || tokens[pos].getType() == TokenType::BANG_EQUAL)) {
            std::string op = tokens[pos].getLexeme();
            pos++; // consumir == o !=
            auto right = parseComparison(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::BinaryOp, op);
            node->addChild(std::move(left));
            node->addChild(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    // Ahora parseExpression será el punto de entrada para expresiones
    static std::unique_ptr<ASTNode> parseExpression(const std::pmr::vector<Token>& tokens, size_t& pos) {
        return parseAssignment(tokens, pos);
    }

    static std::unique_ptr<ASTNode> parseStatement(const std::pmr::vector<Token>& tokens, size_t& pos) {
        // Manejar sentencia return
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::RETURN) {
            pos++; // consumir 'return'
            std::unique_ptr<ASTNode> value = nullptr;
            if (pos < tokens.size() && tokens[pos].getType() != TokenType::SEMICOLON) {
                value = parseExpression(tokens, pos);
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ';' after return value.\n");
            }
            pos++; // consumir ';'
            auto node = std::make_unique<ASTNode>(ASTNode::Type::ReturnStmt, "return");
            if (value) node->addChild(std::move(value));
            return node;
        }

        // Declaración de función: 'fun' IDENTIFIER '()' block
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::FUN) {
            pos++; // consumir 'fun'
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::IDENTIFIER) {
                throw Error(ErrorCodes::ParseError, "Error: Expect function name after 'fun'.\n");
            }
            std::string funcName = tokens[pos].getLexeme();
            pos++; // consumir nombre
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::L_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect '(' after function name.\n");
            }
            pos++; // consumir '('
            // Parsear parámetros
            std::vector<std::string> parameters;
            if (pos < tokens.size() && tokens[pos].getType() != TokenType::R_PAREN) {
                do {
                    if (tokens[pos].getType() != TokenType::IDENTIFIER) {
                        throw Error(ErrorCodes::ParseError, "Error: Expect parameter name.\n");
                    }
                    parameters.push_back(tokens[pos].getLexeme());
                    pos++; // consumir nombre
                    if (pos < tokens.size() && tokens[pos].getType() == TokenType::COMMA) {
                        pos++; // consumir ','
                    } else {
                        break;
                    }
                } while (true);
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ')' after parameters.\n");
            }
            pos++; // consumir ')'
            // Cuerpo de función (debe ser bloque)
            auto body = parseStatement(tokens, pos);
            if (body->getType() != ASTNode::Type::Program || body->getValue() != "block") {
                throw Error(ErrorCodes::ParseError, "Error: Expect function body to be a block.\n");
            }
            auto node = std::make_unique<ASTNode>(ASTNode::Type::Function, funcName);
            // Añadir nodos de parámetros
            for (auto &p : parameters) {
                node->addChild(std::make_unique<ASTNode>(ASTNode::Type::Identifier, p));
            }
            // Añadir cuerpo como último hijo
            node->addChild(std::move(body));
            return node;
        }

        // Sentencia for: 'for' '(' initializer? ';' condition? ';' increment? ')' statement
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::FOR) {
            pos++; // consumir 'for'
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::L_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect '(' after 'for'.\n");
            }
            pos++; // consumir '('
            // Inicializador
            std::unique_ptr<ASTNode> initializer;
            if (tokens[pos].getType() == TokenType::VAR) {
                initializer = parseStatement(tokens, pos);
            } else if (tokens[pos].getType() != TokenType::SEMICOLON) {
                auto initExpr = parseExpression(tokens, pos);
                if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                    throw Error(ErrorCodes::ParseError, "Error: Expect ';' after loop initializer.\n");
                }
                pos++; // consumir ';'
                initializer = std::move(initExpr);
            } else {
                pos++; // consumir ';'
            }
            // Condición
            std::unique_ptr<ASTNode> condition;
            if (tokens[pos].getType() != TokenType::SEMICOLON) {
                condition = parseExpression(tokens, pos);
            } else {
                // falso por defecto
                condition = std::make_unique<ASTNode>(ASTNode::Type::Boolean, "true");
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ';' after loop condition.\n");
            }
            pos++; // consumir ';'
            // Incremento
            std::unique_ptr<ASTNode> increment;
            if (tokens[pos].getType() != TokenType::R_PAREN) {
                increment = parseExpression(tokens, pos);
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ')' after for clauses.\n");
            }
            pos++; // consumir ')'
            // Cuerpo
            auto body = parseStatement(tokens, pos);
            // El cuerpo de for no puede ser var declaration sin bloque
            if (body->getType() == ASTNode::Type::VarDecl) {
                throw Error(ErrorCodes::ParseError, "Error: Expect block after for clauses.\n");
            }
            // Añadir incremento al final del cuerpo
            if (increment) {
                auto block = std::make_unique<ASTNode>(ASTNode::Type::Program, "block");
                block->addChild(std::move(body));
                block->addChild(std::move(increment));
                body = std::move(block);
            }
            // Crear while
            auto loop = std::make_unique<ASTNode>(ASTNode::Type::WhileStmt, "while");
            loop->addChild(std::move(condition));
            loop->addChild(std::move(body));
            // Si hay inicializador, envolver en bloque
            if (initializer) {
                auto block = std::make_unique<ASTNode>(ASTNode::Type::Program, "block");
                block->addChild(std::move(initializer));
                block->addChild(std::move(loop));
                return block;
            }
            return loop;
        }

        // Sentencia if: 'if' '(' expresion ')' statement
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::IF) {
            pos++; // consumir 'if'
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::L_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect '(' after 'if'.\n");
            }
            pos++; // consumir '('
            auto condition = parseExpression(tokens, pos);
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ')' after condition.\n");
            }
            pos++; // consumir ')'
            auto thenBranch = parseStatement(tokens, pos);
            // Ramas else opcional
            std::unique_ptr<ASTNode> elseBranch;
            if (pos < tokens.size() && tokens[pos].getType() == TokenType::ELSE) {
                pos++; // consumir 'else'
                elseBranch = parseStatement(tokens, pos);
            }
            auto node = std::make_unique<ASTNode>(ASTNode::Type::IfStmt, "if");
            node->addChild(std::move(condition));
            node->addChild(std::move(thenBranch));
            if (elseBranch) node->addChild(std::move(elseBranch));
            return node;
        }

        // Sentencia while: 'while' '(' expresion ')' statement
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::WHILE) {
            pos++; // consumir 'while'
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::L_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect '(' after 'while'.\n");
            }
            pos++; // consumir '('
            auto condition = parseExpression(tokens, pos);
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_PAREN) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ')' after condition.\n");
            }
            pos++; // consumir ')'
            auto body = parseStatement(tokens, pos);
            auto node = std::make_unique<ASTNode>(ASTNode::Type::WhileStmt, "while");
            node->addChild(std::move(condition));
            node->addChild(std::move(body));
            return node;
        }

        // Bloque: '{' statements '}'
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::L_BRACE) {
            pos++; // consumir '{'
            auto block = std::make_unique<ASTNode>(ASTNode::Type::Program, "block");
            while (pos < tokens.size() && tokens[pos].getType() != TokenType::R_BRACE) {
                block->addChild(parseStatement(tokens, pos));
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::R_BRACE) {
                throw Error(ErrorCodes::ParseError, "Error at end: Expect '}'\n");
            }
            pos++; // consumir '}'
            return block;
        }

        if (pos < tokens.size() && tokens[pos].getType() == TokenType::VAR) {
            pos++; // consumir 'var'
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::IDENTIFIER) {
                throw Error(ErrorCodes::ParseError, "Error: Expect variable name after 'var'.\n");
            }
            std::string varName = tokens[pos].getLexeme();
            pos++; // consumir nombre
            std::unique_ptr<ASTNode> initExpr;
            if (pos < tokens.size() && tokens[pos].getType() == TokenType::EQUAL) {
                pos++; // consumir '='
                initExpr = parseExpression(tokens, pos);
            }
            

            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ';' after variable declaration.\n");
            }
            pos++; // consumir ';'
            auto node = std::make_unique<ASTNode>(ASTNode::Type::VarDecl, varName);
            if (initExpr) node->addChild(std::move(initExpr));
            return node;
        }
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::PRINT) {
            pos++; // consumir 'print'
            auto expr = parseExpression(tokens, pos);
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ';' after value.\n");
            }
            pos++; // consumir ';'
            auto node = std::make_unique<ASTNode>(ASTNode::Type::PrintStmt, "print");
            node->addChild(std::move(expr));
            return node;
        }

        // Handle return statement
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::RETURN) {
            pos++; // consumir 'return'
            std::unique_ptr<ASTNode> value;
            // Expresión opcional antes de ';'
            if (pos < tokens.size() && tokens[pos].getType() != TokenType::SEMICOLON) {
                value = parseExpression(tokens, pos);
            }
            if (pos >= tokens.size() || tokens[pos].getType() != TokenType::SEMICOLON) {
                throw Error(ErrorCodes::ParseError, "Error: Expect ';' after return value.\n");
            }
            pos++; // consumir ';'
            auto node = std::make_unique<ASTNode>(ASTNode::Type::ReturnStmt, "return");
            if (value) node->addChild(std::move(value));
            return node;
        }

        // Si no es print ni var, puede ser expresión o identificador
        auto expr = parseExpression(tokens, pos);
        if (pos < tokens.size() && tokens[pos].getType() == TokenType::SEMICOLON) {
            pos++; // consumir ';'
        }
        return expr;
    }

    // Nuevo: parsear una secuencia de statements
    static std::unique_ptr<ASTNode> parseProgram(const std::pmr::vector<Token>& tokens, size_t& pos) {
        auto root = std::make_unique<ASTNode>(ASTNode::Type::Program, "program");
        while (pos < tokens.size() && tokens[pos].getType() != TokenType::EOF_OF_FILE) {
            root->addChild(parseStatement(tokens, pos));
        }
        return root;
    }

    // Modificar parse y parseAST para usar parseProgram
    int parse(const std::pmr::vector<Token>& tokens) {
        try {
            size_t pos = 0;
            auto root = parseProgram(tokens, pos);
            // Si solo hay un hijo, imprime ese hijo directamente
            if (root->getChildren().size() == 1) {
                std::cout << root->getChildren()[0]->toString() << std::endl;
            } else {
                std::cout << root->toString() << std::endl;
            }
            return 0;
        } catch (const Error& e) {
            std::cerr << e.message;
            return e.type.code;
        }
    }

    std::unique_ptr<ASTNode> parseAST(const std::pmr::vector<Token>& tokens) {
        size_t pos = 0;
        return parseProgram(tokens, pos);
    }
} // namespace Parser
