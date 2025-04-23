#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "src/commands/Tokenizer.h"
#include "src/commands/Parser.h"

const std::vector<std::string> valid_commands = {"tokenize", "parse", "help"};

bool is_valid_command(const std::string& command) {
    return std::find(valid_commands.begin(), valid_commands.end(), command) != valid_commands.end();
}

std::string read_file_contents(const std::string& filename);

void print_help() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  tokenize <filename>  - Tokenize the input file" << std::endl;
    std::cout << "  parse <filename>     - Parse the input file" << std::endl;
    std::cout << "  help                 - Show this help message" << std::endl;
}

bool validate_arguments(int argc, char* argv[], std::string& command, std::string& filename) {
    if (argc < 2) {
        std::cerr << "Usage: ./Setker <command> <filename>" << std::endl;
        std::cerr << "Or use 'help' for more information." << std::endl;
        return false;
    }

    command = argv[1];

    if (!is_valid_command(command)) {
        std::cerr << "Unknown command: " << command << std::endl;
        std::cerr << "Use 'help' for a list of available commands." << std::endl;
        return false;
    }

    if (command == "help") {
        print_help();
        return false;
    }

    if (argc < 3 && command != "help") {
        std::cerr << "Error: Missing filename argument." << std::endl;
        return false;
    }

    filename = argc >= 3 ? argv[2] : "";
    return true;
}

int handle_command(const std::string& command, const std::string& filename) {
    if (command == "tokenize") {
        std::string file_contents = read_file_contents(filename);
        return Tokenizer::tokenize(file_contents);
    } else if (command == "parse") {
        std::string file_contents = read_file_contents(filename);
        Tokenizer::tokenize(file_contents);
        auto tokens = Tokenizer::getTokens();
        return Parser::parse(tokens);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        std::cerr << "Use 'help' for a list of available commands." << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[]) {
    std::string command, filename;

    if (!validate_arguments(argc, argv, command, filename)) {
        return 1;
    }

    return handle_command(command, filename);
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
