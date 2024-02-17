#include <iostream>

#include "Lexer/Lexer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    const std::string source = "+- # This is a comment!\n */";
    Lexer lex(source);

    const Token* token = lex.getToken();
    while(token->getToken() != eof) {
        std::cout << token->getToken() << std::endl;
        delete token;
        token = lex.getToken();
    }

    return 0;
}
