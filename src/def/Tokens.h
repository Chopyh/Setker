#ifndef TOKENS_H
     #define TOKENS_H
     #include <optional>
     #include <string>

     enum class TokenType {
         // Palabras clave (keywords)
         VAR, IF, ELSE, WHILE, FOR, FUNCTION, RETURN,
         // Identificadores y literales
         IDENTIFIER, STRING, NUMBER,
         // Operadores
         PLUS, MINUS, MULT, DIV, MOD, ASSIGN, EQUAL, DOT, EQUAL_EQUAL, BANG, BANG_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
         // Delimitadores y símbolos
         SEMICOLON, COMMA, COLON, L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET, SLASH,
         // Fin de archivo
         EOF_OF_FILE
     };

     class Token {
     public:
         // Constructor
         Token(TokenType type, std::string lexeme, std::string literal = "");

         // Getters
         [[nodiscard]] TokenType getType() const;
         [[nodiscard]] std::string getLexeme() const;
         [[nodiscard]] std::string getLiteral() const;

         // Métodos
         void print() const;

     private:
         TokenType type;
         std::string lexeme;
         std::string literal;
     };

    #endif //TOKENS_H