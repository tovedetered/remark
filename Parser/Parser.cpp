//
// Created by tobedetered on 2/18/24.
//

#include "Parser.h"

Parser::Parser(Lexer* lex) {
    this->lex = lex;

    currentToken = nullptr;
    peekToken = nullptr;

    nextToken();
    nextToken();
}

Parser::~Parser() {
    //NOTE we do not delete Lexer becasue it is stack allocated
}

bool Parser::checkToken(const TokenType token) const {
    return token == currentToken->getToken();
}

bool Parser::checkPeek(const TokenType token) const {
    return token == peekToken->getToken();
}

void Parser::match(const TokenType token) {
    if(!checkToken(token)) {
        std::cerr << "Expected " << token << ". Got:" << currentToken->getToken() << std::endl;
    }
    nextToken();
}

void Parser::nextToken() {
    delete currentToken;
    currentToken = peekToken;
    peekToken = lex->getToken();
}

void Parser::program() {
    std::cout << "PROGRAM" << std::endl;

    while(!checkToken(eof)) {
        statement();
    }
}

void Parser::statement() {
    switch (currentToken->getToken()) {
    case PRNT:
        std::cout << "STATEMENT: PRINT" << std::endl;
        nextToken();
        if(checkToken(strlit)) {
            //Simple String
            nextToken();
        }
        else {
            //expect expression
            expression();
        }
        break;
    default:
        abort("Invalid Statement" + currentToken->getToken());
        break;
    }

    endLine();
}

void Parser::expression() {

}

void Parser::endLine() {
    std::cout << "END LINE" << std::endl;

    match(ENDLNE);

    while(checkToken(ENDLNE)) {
        nextToken();
    }
}
