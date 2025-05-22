#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "commands/Parser.h"
#include "commands/Tokenizer.h"
#include "commands/Evaluator.h"
#include "commands/Run.h"
#include "def/ErrorCode.h"

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    int exitCode = 0;
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    try {
        if (argc < 3) {
            std::cerr << "Usage: ./your_program <command> <filename>" << std::endl;
            return 1;
        }

        const std::string command = argv[1];

        if (command == "tokenize") {
            std::string file_contents = read_file_contents(argv[2]);

            using namespace Tokenizer;
            exitCode = tokenize(file_contents);
        } else if (command == "parse") {
            std::string file_contents = read_file_contents(argv[2]);

            // Obtener tokens y código de salida
            auto result = Tokenizer::getTokens(file_contents);
            exitCode = result.exitCode;
            if (exitCode == 0) {
                // Use Parser to convert to AST and print
                exitCode = Parser::parse(result.tokens);
            }
        } else if (command == "evaluate") {
            std::string file_contents = read_file_contents(argv[2]);

            auto result = Tokenizer::getTokens(file_contents);
            exitCode = result.exitCode;
            if (exitCode == 0) {                
                exitCode = Evaluator::evaluate(result.tokens);
                std::cout << std::endl;
            }
        } else if (command == "run") {
            std::string file_contents = read_file_contents(argv[2]);
            auto result = Tokenizer::getTokens(file_contents);
            exitCode = result.exitCode;
            if (exitCode == 0) {
                exitCode = Run::run(result.tokens);
            }
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            exitCode =  1;
        }
    } catch (const TokenTree::Error& e) {
        // Error interpretando o parseando
        exitCode = e.type.code;
    }
    return exitCode;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
