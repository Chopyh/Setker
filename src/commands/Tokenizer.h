#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include "../def/Tokens.h"

namespace Tokenizer {
    int tokenize(const std::string& file_contents);
    const std::vector<Token>& getTokens();
}

#endif // TOKENIZER_H