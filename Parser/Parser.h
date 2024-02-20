//
// Created by tobedetered on 2/18/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <iostream>

#include "../Lexer/Lexer.h"


class Parser {
public:
    explicit Parser(Lexer* lex);
    ~Parser();
    [[nodiscard]] bool checkToken(TokenType token) const;
    [[nodiscard]] bool checkPeek(TokenType token) const;
    void match(TokenType token);
    void nextToken();

    static void abort(const std::string& message) {
        std::cerr << "Parser Error: " + message << std::endl;
        exit(-1);
    }

    void program();
    void statement();
    void expression();
    void endLine();
private:
    Token* currentToken;
    Token* peekToken;
    Lexer* lex;
};



#endif //PARSER_H
