#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "commands/Tokenizer.h"

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    int exitCode = 0;
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);

        using namespace Tokenizer;
        exitCode = tokenize(file_contents);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        exitCode =  1;
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
