/**
 * @file ErrorCode.h
 * @brief Sistema de manejo de errores y códigos de salida
 * @author Javier
 * @date 2025
 * 
 * Este archivo define un sistema robusto para el manejo de errores
 * del intérprete, incluyendo tipos de error específicos, códigos
 * de salida estándar y excepciones personalizadas.
 */

#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <string>

namespace TokenTree {
    /**
     * @struct ErrorType
     * @brief Estructura que combina nombre y código de error
     * 
     * Agrupa un nombre descriptivo del error con su código numérico
     * correspondiente, facilitando el manejo consistente de errores.
     */
    struct ErrorType {
        std::string name;   ///< Nombre descriptivo del error
        int code;          ///< Código numérico del error
    };

    /**
     * @struct Error
     * @brief Excepción genérica del intérprete usando ErrorType
     * 
     * Excepción personalizada que incluye tanto el tipo de error
     * como un mensaje descriptivo específico del contexto.
     */
    struct Error {
        ErrorType type;         ///< Tipo de error
        std::string message;    ///< Mensaje descriptivo del error
        
        /**
         * @brief Constructor de Error
         * @param type Tipo de error predefinido
         * @param msg Mensaje específico (opcional, usa type.name por defecto)
         */
        Error(const ErrorType &type, const std::string &msg = "")
            : type(type), message(msg.empty() ? type.name : msg) {}
    };

    /**
     * @namespace ErrorCodes
     * @brief Definiciones de todos los códigos de error del intérprete
     * 
     * Contiene constantes para todos los tipos de errores que puede
     * generar el intérprete, organizados por categoría y con códigos
     * de salida estándar apropiados.
     */
    namespace ErrorCodes {
        // Errores de tiempo de ejecución (Runtime Errors) - Código 70
        inline const ErrorType InvalidAssignmentTarget     {"InvalidAssignmentTarget",     70}; ///< Destino de asignación inválido
        inline const ErrorType OperandMustBeNumber         {"OperandMustBeNumber",         70}; ///< Operando debe ser numérico
        inline const ErrorType OperandsMustBeNumbers       {"OperandsMustBeNumbers",       70}; ///< Operandos deben ser numéricos
        inline const ErrorType ArgumentCountMismatch       {"ArgumentCountMismatch",       70}; ///< Número incorrecto de argumentos
        inline const ErrorType CallOnNonFunction           {"CallOnNonFunction",           70}; ///< Intento de llamar a no-función
        inline const ErrorType RuntimeError                {"RuntimeError",                70}; ///< Error genérico de ejecución
        
        // Errores de parsing (Parse Errors) - Código 65
        inline const ErrorType ParseError                  {"ParseError",                  65}; ///< Error de análisis sintáctico
        
        // Códigos de salida estándar:
        // 0  - Éxito
        // 65 - Errores de análisis/sintaxis
        // 70 - Errores de tiempo de ejecución
    }
}

#endif // ERRORCODE_H
