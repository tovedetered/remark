//
// Created by tobedetered on 2/17/24.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Token.h"
#include "TokenType.h"

class Token {
public:
    Token(const std::string& text, TokenType token);
    [[nodiscard]] TokenType getToken() const;
    [[nodiscard]] std::string getText() const;

private:
    TokenType token;
    std::string text;
    TokenType token_;
};



#endif //TOKEN_H
