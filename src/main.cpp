/**
 * @file main.cpp
 * @brief Punto de entrada principal del intérprete Setker
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene la función main que maneja la línea de comandos
 * y coordina las diferentes fases del procesamiento del lenguaje Setker:
 * tokenización, parsing, evaluación y ejecución.
 */

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

/**
 * @brief Lee el contenido completo de un archivo de texto
 * @param filename Ruta del archivo a leer
 * @return std::string Contenido del archivo como cadena
 * @throws std::exit(1) Si no se puede abrir el archivo
 * 
 * Esta función abre un archivo en modo texto, lee todo su contenido
 * en un buffer de cadena y devuelve el resultado. Si el archivo no
 * se puede abrir, termina el programa con código de error 1.
 */
std::string read_file_contents(const std::string& filename);

/**
 * @brief Muestra información de ayuda detallada sobre el intérprete
 * 
 * Imprime en la salida estándar una descripción completa de todos los
 * comandos disponibles, su sintaxis, propósito y ejemplos de uso.
 * Esta función se llama cuando el usuario ejecuta './setker help'.
 */
void print_help();

/**
 * @brief Función principal del intérprete Setker
 * @param argc Número de argumentos de línea de comandos
 * @param argv Array de argumentos de línea de comandos
 * @return int Código de salida (0 = éxito, >0 = error)
 * 
 * Maneja la línea de comandos y ejecuta la operación correspondiente:
 * - tokenize: Análisis léxico y muestra de tokens
 * - parse: Análisis sintáctico y construcción del AST
 * - evaluate: Evaluación de expresiones paso a paso
 * - run: Ejecución completa del programa
 * - help: Muestra información de ayuda
 * 
 * La función coordina las diferentes fases del procesamiento del lenguaje,
 * maneja errores y devuelve códigos de salida apropiados.
 */
int main(int argc, char *argv[]) {
    int exitCode = 0;
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;    try {
        if (argc < 2) {
            std::cerr << "Usage: ./your_program <command> [filename]" << std::endl;
            std::cerr << "Use 'help' command for more information." << std::endl;
            return 1;
        }

        const std::string command = argv[1];

        if (command == "help") {
            print_help();
            return 0;
        }

        if (argc < 3) {
            std::cerr << "Usage: ./your_program <command> <filename>" << std::endl;
            std::cerr << "Use 'help' command for more information." << std::endl;
            return 1;
        }

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
            }        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            std::cerr << "Use 'help' command for more information." << std::endl;
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
    file.close();    return buffer.str();
}

void print_help() {
    std::cout << "Setker - Intérprete de lenguaje de programación" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << std::endl;
    std::cout << "USO:" << std::endl;
    std::cout << "  ./setker <comando> <archivo>" << std::endl;
    std::cout << "  ./setker help" << std::endl;
    std::cout << std::endl;
    std::cout << "COMANDOS DISPONIBLES:" << std::endl;
    std::cout << std::endl;
    
    std::cout << "  tokenize <archivo>" << std::endl;
    std::cout << "    Analiza léxicamente el archivo fuente y muestra todos los tokens identificados." << std::endl;
    std::cout << "    Útil para debuggear problemas de sintaxis y entender cómo el intérprete" << std::endl;
    std::cout << "    interpreta el código fuente a nivel de tokens." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  parse <archivo>" << std::endl;
    std::cout << "    Analiza sintácticamente el archivo fuente y construye el Árbol de Sintaxis" << std::endl;
    std::cout << "    Abstracta (AST). Muestra la estructura jerárquica del programa." << std::endl;
    std::cout << "    Útil para verificar que la sintaxis del código es correcta y visualizar" << std::endl;
    std::cout << "    cómo se estructura el programa internamente." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  evaluate <archivo>" << std::endl;
    std::cout << "    Evalúa las expresiones del archivo fuente y muestra los resultados." << std::endl;
    std::cout << "    Procesa el código paso a paso mostrando el valor de cada expresión" << std::endl;
    std::cout << "    evaluada. Útil para entender el flujo de evaluación del programa." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  run <archivo>" << std::endl;
    std::cout << "    Ejecuta completamente el programa contenido en el archivo fuente." << std::endl;
    std::cout << "    Este es el comando principal para ejecutar programas escritos en Setker." << std::endl;
    std::cout << "    Ejecuta todas las instrucciones y muestra la salida final del programa." << std::endl;
    std::cout << std::endl;
    
    std::cout << "  help" << std::endl;
    std::cout << "    Muestra esta información de ayuda con la descripción de todos los comandos" << std::endl;
    std::cout << "    disponibles y sus propósitos." << std::endl;
    std::cout << std::endl;
    
    std::cout << "EJEMPLOS DE USO:" << std::endl;
    std::cout << "  ./setker run examples/factorial.stk" << std::endl;
    std::cout << "  ./setker tokenize examples/arithmetic.stk" << std::endl;
    std::cout << "  ./setker parse examples/functions.stk" << std::endl;
    std::cout << "  ./setker evaluate examples/control_flow.stk" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Para más información sobre el lenguaje Setker, consulte la documentación" << std::endl;
    std::cout << "en el archivo README.md del proyecto." << std::endl;
}
