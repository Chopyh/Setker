/**
 * @file Tokenizer.cpp
 * @brief Implementación del analizador léxico para Setker
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene la implementación completa del analizador léxico,
 * incluyendo el reconocimiento de tokens, manejo de comentarios, cadenas,
 * números, operadores y palabras clave del lenguaje Setker.
 */

#include "Tokenizer.h"
#include "Parser.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "../def/Tokens.h"
#include "../def/Keywords.h"

namespace Tokenizer {
    // Variables globales del tokenizer
    auto tokens = std::pmr::vector<Token>();  ///< Vector de tokens identificados
    int line = 1;                             ///< Número de línea actual
    int exitCode = 0;                         ///< Código de salida del proceso

    /**
     * @brief Verifica si un carácter es una letra o guión bajo
     * @param c Carácter a verificar
     * @return bool true si es letra o '_', false en caso contrario
     */
    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    /**
     * @brief Verifica si un carácter es alfanumérico
     * @param c Carácter a verificar
     * @return bool true si es letra, dígito o '_', false en caso contrario
     */
    bool isAlphaNumeric(char c) {
        return isLetter(c) || isdigit(c);
    }

    /**
     * @brief Verifica si un carácter es un dígito
     * @param c Carácter a verificar
     * @return bool true si es dígito, false en caso contrario
     */
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    /**
     * @brief Procesa comentarios multi-línea delimitados por <| y |>
     * @param file_contents Contenido del archivo
     * @param i Índice actual en el archivo (se modifica por referencia)
     */
    void valorateWhitespaces(const std::string &file_contents, int &i) {
        for (int j = i + 1; j < file_contents.size(); j++) {
            if (file_contents[j] == '|' && file_contents[j + 1] == '>') {
                i = j + 1;
                break;
            }
        }
    }

    /**
     * @brief Procesa identificadores y palabras clave
     * @param file_contents Contenido del archivo
     * @param i Índice actual en el archivo (se modifica por referencia)
     */
    void valorateString(const std::string &file_contents, int &i) {
        for (int j = i + 1; j <= file_contents.size(); j++) {
            if (j == file_contents.size() || file_contents[j] == ' ' || file_contents[j] == '\n' || file_contents[j] == '\t' || !isAlphaNumeric(file_contents[j])) {
                auto keyword = file_contents.substr(i, j - i);
                tokens.emplace_back(Keywords::valorateKeyword(keyword), keyword);
                i = j - 1;
                break;
            }
        }
    }

    /**
     * @brief Procesa literales numéricos (enteros y decimales)
     * @param file_contents Contenido del archivo
     * @param i Índice actual en el archivo (se modifica por referencia)
     */
    void valorateNumber(const std::string &file_contents, int &i) {
        for (int j = i + 1; j <= file_contents.size(); j++) {
            if (j == file_contents.size() || file_contents[j] == ' ' || file_contents[j] == '\n' || file_contents[j] == '\t' || !isDigit(file_contents[j])) {
                if (file_contents[j] == '.') continue;
                std::string lexeme = file_contents.substr(i, j - i); // Toma el número tal cual del texto
                auto number = std::stod(lexeme); // El literal sigue siendo double
                tokens.emplace_back(TokenType::NUMBER, lexeme, number);
                i = j - 1;
                break;
            }
        }
    }

    /**
     * @brief Imprime todos los tokens identificados
     */
    void printTokens() {
        for (auto &token : tokens) {
            std::cout << token.print() << std::endl;
        }
    }

    /**
     * @brief Función principal de tokenización
     * @param file_contents Contenido del archivo a tokenizar
     * 
     * Recorre carácter por carácter el contenido del archivo,
     * identificando y clasificando tokens según las reglas
     * léxicas del lenguaje Setker.
     */
    void valorateTokens(const std::string& file_contents) {
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
                case '%':
                    tokens.emplace_back(TokenType::MOD, "%");
                    break;
                case '=':
                    if (file_contents[i + 1] == '=') {
                        tokens.emplace_back(TokenType::EQUAL_EQUAL, "==");
                        i++;
                    } else {
                        tokens.emplace_back(TokenType::EQUAL, "=");
                    }
                    break;
                case '"': {
                    int j = i + 1;
                    std::string str;
                    for (; j < file_contents.size(); ++j) {
                        if (file_contents[j] == '"') {
                            break;
                        }
                        if (file_contents[j] == '\n') {
                            line += 1;
                        }
                        str += file_contents[j];
                    }
                    if (j >= file_contents.size()) {
                        std::cerr << "[line " << line << "] Error: Unterminated string." << std::endl;
                        exitCode = 65;
                        i = j;
                        break;
                    }
                    tokens.emplace_back(TokenType::STRING, str, str);
                    i = j;
                    break;
                }
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
                    } else if (file_contents[i + 1] == '|'){


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
    }

    int tokenize(const std::string& file_contents) {
        valorateTokens(file_contents);

        printTokens();

        return exitCode;
    }

    void parse(const std::pmr::vector<Token>& tokens) {
        Parser::parse(tokens);
    }

    Result getTokens(const std::string& file_contents) {
        // Reiniciar estado
        tokens.clear();
        exitCode = 0;
        valorateTokens(file_contents);
        return { tokens, exitCode };
    }
}