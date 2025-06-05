/**
 * @file Run.h
 * @brief Módulo de ejecución completa de programas Setker
 * @author Javier
 * @date 2025
 * 
 * Este archivo define la interfaz para la ejecución completa de programas
 * escritos en Setker. Integra todas las fases del intérprete: tokenización,
 * parsing y evaluación para proporcionar ejecución completa de código.
 */

#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../def/Tokens.h"

/**
 * @namespace Run
 * @brief Espacio de nombres para la ejecución completa de programas
 * 
 * Proporciona funcionalidad de alto nivel para ejecutar programas
 * completos de Setker, integrando todas las fases del procesamiento
 * y manejando la ejecución de instrucciones como print, funciones
 * y estructuras de control.
 */
namespace Run {
    /**
     * @brief Ejecuta un programa completo desde tokens
     * @param tokens Vector de tokens que representa el programa
     * @return int Código de salida (0 = éxito, >0 = error)
     * 
     * Toma una secuencia de tokens previamente generada por el tokenizer
     * y ejecuta el programa completo:
     * 1. Construye el AST usando el parser
     * 2. Evalúa el programa completo usando el evaluador
     * 3. Maneja errores y devuelve códigos de salida apropiados
     * 
     * Esta función es utilizada por el comando 'run' del intérprete
     * y representa la funcionalidad principal para ejecutar programas
     * escritos en Setker.
     * 
     * A diferencia del comando 'evaluate' que muestra el resultado
     * de expresiones, 'run' ejecuta programas completos incluyendo
     * efectos secundarios como instrucciones print, modificación de
     * variables y ejecución de funciones.
     */
    int run(const std::pmr::vector<Token>& tokens);
}
