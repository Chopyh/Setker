#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../def/Tokens.h"

namespace Run {
    // Ejecuta el código fuente (tokens) y soporta print y expresiones
    int run(const std::pmr::vector<Token>& tokens);
}
