/**
 * @file Environment.cpp
 * @brief Implementación del sistema de entornos para variables
 * @author Javier
 * @date 2025
 * 
 * Este archivo contiene la implementación de la clase Environment,
 * proporcionando funcionalidad para crear, buscar y modificar variables
 * en un sistema de entornos anidados con scoping léxico.
 */

#include "Environment.h"
#include <stdexcept>

namespace TokenTree {
    Environment::Environment() : enclosing(nullptr) {}
    Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

    void Environment::define(const std::string& name, const Value& value) {
        values[name] = value;
    }

    Environment::Value Environment::get(const std::string& name) const {
        auto it = values.find(name);
        if (it != values.end()) {
            return it->second;
        }
        if (enclosing) {
            return enclosing->get(name);
        }
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }

    void Environment::assign(const std::string& name, const Value& value) {
        auto it = values.find(name);
        if (it != values.end()) {
            values[name] = value;
            return;
        }
        if (enclosing) {
            enclosing->assign(name, value);
            return;
        }
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }
}
