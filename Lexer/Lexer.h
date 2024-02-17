//
// Created by tobedetered on 2/17/24.
//

#ifndef LEXER_H
#define LEXER_H
#include <string>

#include "Token.h"


class Lexer {
public:
    explicit Lexer(std::string source);
    void nextChar();
    [[nodiscard]] char peek() const;
    [[nodiscard]] char getCurrentChar() const;
    static void abort(const std::string& message);
    void skipWhitespace();
    void skipComment();
    Token* getToken();
private:
    std::string source;
    char currentChar{};
    int currentPos;
};



#endif //LEXER_H
