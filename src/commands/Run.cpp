#include "Run.h"
#include "Parser.h"
#include "Evaluator.h"
#include "../def/ErrorCode.h"
#include <iostream>
#include <memory_resource>

namespace Run {
    int run(const std::pmr::vector<Token>& tokens) {
        try {
            // Usar el AST del parser
            auto ast = Parser::parseAST(tokens);
            // Evaluar el AST (soporta print y expresiones)
            Evaluator::evalNode(ast.get());
            return 0;
        } catch (const Evaluator::Error& e) {
            std::cerr << e.message << std::endl;
            return e.type.code;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
}
