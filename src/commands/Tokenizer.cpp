#include "Tokenizer.h"

#include <iostream>
#include <vector>

#include "../def/Tokens.h"
#include "../def/Keywords.h"

namespace Tokenizer {
    auto tokens = std::vector<Token>();
    int line = 1;
    int exitCode = 0;

    void valorateWhitespaces(const std::string &file_contents, int &i) {
        for (int j = i + 1; j < file_contents.size(); j++) {
            if (file_contents[j] == '|' && file_contents[j + 1] == '>') {
                i = j + 1;
                break;
            }
        }
    }
    void valorateString(const std::string &file_contents, int &i) {
        for (int j = i + 1; j < file_contents.size(); j++) {
            if (file_contents[j] == ' ' || file_contents[j] == '\n' || file_contents[j] == '\t') {
                auto keyword = file_contents.substr(i, j - i);

                tokens.emplace_back(Keywords::valorateKeyword(keyword), keyword);
                i = j - 1;
                break;
            }
        }
    }
    void valorateNumber(const std::string &file_contents, int &i) {
        for (int j = i + 1; j < file_contents.size(); j++) {
            if (file_contents[j] == ' ' || file_contents[j] == '\n' || file_contents[j] == '\t') {
                auto number = std::stod(file_contents.substr(i, j - i));

                tokens.emplace_back(TokenType::NUMBER, std::to_string(number), std::to_string(number));
                i = j - 1;
                break;
            }
        }
    }
    bool isLetter(const char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    bool isDigit(const char c) {
        return c >= '0' && c <= '9';
    }
    void printTokens() {
        for (auto &token : tokens) {
            token.print();
        }
    }

    int tokenize(const std::string& file_contents) {
        for (int i = 0; i < file_contents.size(); i++) {
            switch (file_contents[i]) {
                case '+':
                    tokens.emplace_back(TokenType::PLUS, "+");
                    break;
                case '-':
                    tokens.emplace_back(TokenType::MINUS, "-");
                    break;
                case '*':
                    tokens.emplace_back(TokenType::MULT, "*");
                    break;
                case '/':
                    if (file_contents[i + 1] == '/') {
                        for (int j = i + 1; j <= file_contents.size(); j++) {
                            if (file_contents[j] == '\n' || j == file_contents.size()) {
                                if (file_contents[j] == '\n') line += 1;
                                i = j;
                                break;
                            }
                        }
                    } else tokens.emplace_back(TokenType::SLASH, "/");
                    break;
                case '=':
                    if (file_contents[i + 1] == '=') {
                        tokens.emplace_back(TokenType::EQUAL_EQUAL, "==");
                        i++;
                    } else {
                        tokens.emplace_back(TokenType::EQUAL, "=");
                    }
                    break;
                case '"':
                    for (int j = i + 1; j <= file_contents.size(); j++) {
                        if (file_contents[j] == '"') {
                            tokens.emplace_back(TokenType::STRING, file_contents.substr(i + 1, j - i - 1));
                            break;
                        }
                        if (file_contents[j] == '\n' || j == file_contents.size()) {
                            std::cerr << "[line " << line << "] Error: Unterminated string." << std::endl;
                            exitCode = 65;
                            break;
                        }
                    }
                    break;
                case ';':
                    tokens.emplace_back(TokenType::SEMICOLON, ";");
                    break;
                case ',':
                    tokens.emplace_back(TokenType::COMMA, ",");
                    break;
                case '.':
                    tokens.emplace_back(TokenType::DOT, ".");
                    break;
                case ':':
                    tokens.emplace_back(TokenType::COLON, ":");
                    break;
                case '(':
                    tokens.emplace_back(TokenType::L_PAREN, "(");
                    break;
                case ')':
                    tokens.emplace_back(TokenType::R_PAREN, ")");
                    break;
                case '{':
                    tokens.emplace_back(TokenType::L_BRACE, "{");
                    break;
                case '}':
                    tokens.emplace_back(TokenType::R_BRACE, "}");
                    break;
                case '[':
                    tokens.emplace_back(TokenType::L_BRACKET, "[");
                    break;
                case ']':
                    tokens.emplace_back(TokenType::R_BRACKET, "]");
                    break;
                case '!':
                    if (file_contents[i + 1] == '=') {
                        tokens.emplace_back(TokenType::BANG_EQUAL, "!=");
                        i++;
                    } else {
                        tokens.emplace_back(TokenType::BANG, "!");
                    }
                    break;
                case '>':
                    if (file_contents[i + 1] == '=') {
                        tokens.emplace_back(TokenType::GREATER_EQUAL, ">=");
                        i++;
                    } else {
                        tokens.emplace_back(TokenType::GREATER, ">");
                    }
                    break;
                case '<':
                    if (file_contents[i + 1] == '=') {
                        tokens.emplace_back(TokenType::LESS_EQUAL, "<=");
                        i++;
                    } else if (file_contents[i + 1] == '|') {
                        valorateWhitespaces(file_contents, i);
                    } else {
                        tokens.emplace_back(TokenType::LESS, "<");
                    }
                    break;
                case '\n':
                    line += 1;
                    break;
                case ' ':
                case '\t':
                    break;
                default:
                    if (isLetter(file_contents[i])) {
                        valorateString(file_contents, i);
                    } else if (isDigit(file_contents[i])) {
                        valorateNumber(file_contents, i);
                    } else {
                        std::cerr << "[line " << line << "] Error: Unexpected character: " << file_contents[i] << std::endl;
                        exitCode = 65;
                    }

            }
        }
        tokens.emplace_back(TokenType::EOF_OF_FILE, "");

        printTokens();

        return exitCode;
    }
}