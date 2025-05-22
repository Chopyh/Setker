#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <memory_resource>

#include "../def/Tokens.h"

namespace Tokenizer {
    // Resultado de tokenización con tokens y código de salida
    struct Result {
        std::pmr::vector<Token> tokens;
        int exitCode;
    };

    int tokenize(const std::string& file_contents);
    Result getTokens(const std::string& file_contents);
}

#endif // TOKENIZER_H