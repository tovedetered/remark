//
// Created by tobedetered on 2/17/24.
//

#include "Lexer.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>

Lexer::Lexer(std::string source) {
    keyident = new KeywordIdentifier;
    {
        std::ifstream ifs("keyword_ident.txt");
        boost::archive::text_iarchive ia(ifs);
        ia >> *keyident;
        ifs.close();
    }
    this->source = std::move(source);
    this->currentChar = ' ';
    this->currentPos = -1;
    nextChar();

    startIf = false;
    startWhile = false;
}

Lexer::~Lexer() {
    delete keyident;
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
    char switchChar = currentChar;
    if(std::isdigit(currentChar)) switchChar = '7'; // to do numbers and digits in a switch
    else if(std::isalnum(currentChar)) switchChar = 'z'; //to do keywords

    switch (switchChar) {
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
            token = new Token(std::string() + currentChar, ENDLNE);
        }
        break;
    case '\"': {
        nextChar();
        const int startPos = currentPos;
        int length = 1;
        //For now just using c's printf to print so no special characters are allowed
        while(currentChar != '\"') {
            if(currentChar == '\r' || currentChar == '\n' || currentChar == '\t' || currentChar == '\\' || currentChar == '%') {
                abort("Illegal Character in string! ");
            }
            nextChar();
            length ++;
        }
        std::string tokenText = source.substr(startPos, length - 1);
        token = new Token(tokenText, strlit);
        break;
    }
    case '7': {
        //if there is a digit has to be a number, so get all consec digits and decimal if there is one
        int startPos = currentPos;
        int length = 1;
        while(std::isdigit(peek())) {
            nextChar();
            length++;
        }
        //if decimal
        if(peek() == '.') {
            nextChar();
            length++;
            //have to have a digit after TODO: Add f for floating points
            if(!std::isdigit(peek())) {
                //Error Here
                abort("Illegal Character in Number ");
            }
            while(std::isdigit(peek())) {
                nextChar();
                length++;
            }
        }

        std::string tokenText = source.substr(startPos, length);
        token = new Token(tokenText, number);
        break;
    }
    case 'z': {
        const int startPos = currentPos;
        int length = 0;
        while(std::isalpha(currentChar)) {
            nextChar();
            length++;
        }

        //Check if token is in list of keywords
        const std::string tokenText = source.substr(startPos, length);
        const TokenType keyword = keyident->getKeyword(tokenText);
        if(keyword == IF) {
            startIf = true;
        }
        else if(keyword == WHILE) {
            startWhile = true;
        }
        token = new Token(tokenText, keyword);
        currentPos --;
        break;
    }
    case ';':
        token = new Token(strtoken, ENDLNE);
        break;
    case '{':
        token = new Token(strtoken, BEGIN);
        break;
    case '}':
        token = new Token(strtoken, END);
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
