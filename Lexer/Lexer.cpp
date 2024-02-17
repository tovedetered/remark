//
// Created by tobedetered on 2/17/24.
//

#include "Lexer.h"

#include <cstring>
#include <iostream>

Lexer::Lexer(std::string source) {
    this->source = std::move(source);
    this->currentChar = ' ';
    this->currentPos = -1;
    nextChar();
}

void Lexer::nextChar() {
    currentPos ++;
    if(currentPos >= source.length()) {
        currentChar = '\0';
    }
    else {
        currentChar = source[currentPos];
    }
}

char Lexer::peek() const {
    if(const int peekPos = currentPos + 1;
        peekPos == source.length()) {
        return '\0';
    }
    return source[currentPos + 1];
}

char Lexer::getCurrentChar() const {return currentChar;}

void Lexer::abort(const std::string& message) {
    std::cerr << "Lexing Error: " + message << std::endl;
    exit(-1);
}

void Lexer::skipWhitespace() {
    while(currentChar == ' ' || currentChar == '\t' || currentChar == '\r' || currentChar == '\n') {
        nextChar();
    }
}

void Lexer::skipComment() {
    if(currentChar == '#') {
        while(currentChar != '\n') {
            nextChar();
        }
    }
    skipWhitespace();
}

Token* Lexer::getToken() {
    skipWhitespace();
    skipComment();

    Token* token = nullptr;
    std::string strtoken(1, currentChar);
    switch (currentChar) {
    case '+':
        token = new Token(strtoken, PLUS);
        break;
    case '-':
        token = new Token(strtoken, MINUS);
        break;
    case '*':
        token = new Token(strtoken, ASTERISK);
        break;
    case '/':
        token = new Token(strtoken, SLASH);
        break;
    case '=':
        if(peek() == '=') {
            char lastChar = currentChar;
            nextChar();
            token = new Token(std::string() + lastChar + currentChar, EQEQ);
        }
        else {
            token = new Token(strtoken, EQ);
        }
        break;
    case '>':
        if(peek() == '=') {
            char lastChar = currentChar;
            nextChar();
            token = new Token(std::string() + lastChar + currentChar, GTEQ);
        }
        else {
            token = new Token(strtoken, GT);
        }
        break;
    case '<':
        if(peek() == '=') {
            char lastChar = currentChar;
            nextChar();
            token = new Token(std::string() + lastChar + currentChar, LTEQ);
        }
        else {
            token = new Token(strtoken, LT);
        }
        break;
    case '!':
        if(peek() == '=') {
            char lastChar = currentChar;
            nextChar();
            token = new Token(std::string() + lastChar + currentChar, NOTEQ);
        }
        else {
            abort("Expected !=, got !" + peek());
        }
        break;
    case '\0':
        token = new Token(strtoken, eof);
        break;
    default:
        std::string baseMessage = "Unkown Token: " + std::string(1, currentChar);
        abort(baseMessage);
        break;
    }

    nextChar();
    return token;
}
