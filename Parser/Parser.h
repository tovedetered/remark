//
// Created by tobedetered on 2/18/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <unordered_set>

#include "../Emitter/Emitter.h"
#include "../Lexer/Lexer.h"


class Parser {
public:
    explicit Parser(Lexer* lex, Emitter* emit);
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
    [[nodiscard]] bool isComparisonOperator() const;
    void statement();
    void comparison();
    void expression();
    void term();
    void unary();
    void primary();
    void endLine();
private:
    Token* currentToken;
    Token* peekToken;
    Lexer* lex;
    Emitter* emit;

    std::unordered_set<std::string> symbolSet; //Vairables
    std::unordered_set<std::string> labels;
    std::unordered_set<std::string> labelsGoneTo;
};



#endif //PARSER_H
