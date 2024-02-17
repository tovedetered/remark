//
// Created by tobedetered on 2/17/24.
//

#include "Token.h"

Token::Token(const std::string& text, const TokenType token): token_(token) {
    this->text = text;
    this->token = token;
}

TokenType Token::getToken() const { return token; }

std::string Token::getText() const { return text; }
