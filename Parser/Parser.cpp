//
// Created by tobedetered on 2/18/24.
//

#include "Parser.h"

Parser::Parser(Lexer* lex, Emitter* emit) {
    this->lex = lex;
    this->emit = emit;

    currentToken = nullptr;
    peekToken = nullptr;

    nextToken();
    nextToken();
}

Parser::~Parser() {
    //NOTE we do not delete Lexer or emitter becasue they are stack allocated
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
        exit(-1);
    }
    nextToken();
}

void Parser::nextToken() {
    delete currentToken;
    currentToken = peekToken;
    peekToken = lex->getToken();
}

void Parser::program() {

    emit->headerLine("#include <stdio.h>");
    emit->headerLine("int main(void){");

    while(checkToken(ENDLNE)) {
        nextToken();
    }

    while(!checkToken(eof)) {
        statement();
    }

    emit->emitLine("return 0;");
    emit->emitLine("}");

    for(const auto& label : labelsGoneTo) {
        if(!labels.contains(label)) {
            abort("Attempting to GOTO undeclared label: " + label);
        }
    }
}

bool Parser::isComparisonOperator() const {
    return checkToken(GT) || checkToken(GTEQ) || checkToken(LT) || checkToken(LTEQ) || checkToken(EQEQ) || checkToken(NOTEQ);
}

void Parser::statement() {
    switch (currentToken->getToken()) {
    case PRNT:
        //prnt (statement|strlit) end
        nextToken();
        if(checkToken(strlit)) {
            //Simple String
            emit->emitLine("printf(\"" + currentToken->getText() + "\\n\");");
            nextToken();
        }
        else {
            //expect expression
            emit->emitLine(std::string() + "printf(\"%" + ".2f\\n\", (float)(");
            expression();
            emit->emitLine("));");
        }
        break;
    case IF:
        //"IF" comparison "BEGIN" {statement} "END"
        nextToken();
        //Compiler will accept if i > 1 , if(i>1) , if (i>1), etc.
        if(currentToken->getToken() == STRTMOD) nextToken();
        emit->emit("if(");
        comparison();
        if(currentToken->getToken() == ENDMOD) nextToken();
        match(BEGIN);
        emit->emitLine("){");
        while(!checkToken(END)) {
            statement();
        }
        match(END);
        emit->emitLine("}");
        nextToken();

        ELSECHAIN:
        if(currentToken->getToken() == ELSE) {
            nextToken();
            emit->emitLine("else{");
            match(BEGIN);
            while (!checkToken(END)) {
                statement();
            }
            match(END);
            emit->emitLine("}");
        }
        else if(currentToken->getToken() == ELSEIF) {
            nextToken();
            if(currentToken->getToken() == STRTMOD) nextToken();
            emit->emit("else if(");
            comparison();
            if(currentToken->getToken() == ENDMOD) nextToken();
            match(BEGIN);
            emit->emitLine("){");
            while(!checkToken(END)) {
                statement();
            }
            match(END);
            emit->emitLine("}");
        }
        if(peekToken->getToken() == ELSE || peekToken->getToken() == ELSEIF) {
            nextToken();
            goto ELSECHAIN;
        }
        break;
    case WHILE:
        //"WHILE" comparison "BEGIN" "ENDLNE" {statement} ("END" | "ENDLNE") "ENDLNE"
        nextToken();
        if(currentToken->getToken() == STRTMOD) nextToken();
        emit->emit("while(");
        comparison();
        if(currentToken->getToken() == ENDMOD) nextToken();
        match(BEGIN);
        emit->emitLine("){");
        while(!checkToken(END)) {
            statement();
        }
        match(END);
        emit->emitLine("}");
        break;
    case LBL:
        //lbl ident
        nextToken();
        if(labels.contains(currentToken->getText())) {
            abort("Label already exists: " + currentToken->getText());
        }
        labels.emplace(currentToken->getText());
        emit->emitLine(currentToken->getText() + ":");
        match(ident);
        break;
    case GOTO:
        //goto ident
        nextToken();
        labelsGoneTo.emplace(currentToken->getText());
        emit->emitLine("goto " + currentToken->getText() + ";");
        match(ident);
        break;
    case LET:
        nextToken();
        if(!symbolSet.contains(currentToken->getText())) {
            symbolSet.emplace(currentToken->getText());
            emit->headerLine("float " + currentToken->getText() + ";");
        }
        emit->emit(currentToken->getText() + " = ");
        match(ident);
        match(EQ);
        expression();
        emit->emitLine(";");
        break;
    case INPT:
        nextToken();
        if(!symbolSet.contains(currentToken->getText())) {
            symbolSet.emplace(currentToken->getText());
            emit->headerLine("float " + currentToken->getText() + ";");
        }
        //emit scanf but also handle invalid input by setting it to 0
        emit->emitLine(std::string() + "if(0 == scanf(\"%" + "f\", &" + currentToken->getText() + ")) {");
        emit->emitLine(currentToken->getText() + " = 0;");
        emit->emit("scanf(\"%");
        emit->emitLine("*s\");");
        emit->emitLine("}");
        match(ident);
        break;
    default:
        abort("Invalid Statement at " + currentToken->getText());
        break;
    }

    endLine();
}

//expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression)+
void Parser::comparison() {

    expression();

    if(isComparisonOperator()) {
        emit->emit(currentToken->getText());
        nextToken();
        expression();
    }
    else {
        abort("Expected comparison operator at: " + currentToken->getText());
    }

    while(isComparisonOperator()) {
        emit->emit(currentToken->getText());
        nextToken();
        expression();
    }
}

void Parser::expression() {

    term();
    //can have 0 or more next things
    while(checkToken(PLUS) || checkToken(MINUS)) {
        emit->emit(currentToken->getText());
        nextToken();
        term();
    }
}

void Parser::term() {

    unary();
    while(checkToken(ASTERISK) || checkToken(SLASH)) {
        emit->emit(currentToken->getText());
        nextToken();
        unary();
    }
}

void Parser::unary() {

    //Optional Unary +/-
    if(checkToken(PLUS) || checkToken(MINUS)) {
        emit->emit(currentToken->getText());
        nextToken();
    }
    primary();
}

void Parser::primary() {

    if(checkToken(number)) {
        emit->emit(currentToken->getText());
        nextToken();
    }
    else if(checkToken(ident)) {
        if(!symbolSet.contains(currentToken->getText())) {
            abort("Referencing variable before assignment: " + currentToken->getText());
        }
        emit->emit(currentToken->getText());
        nextToken();
    }
    else {
        abort("Unexpected Token at " + currentToken->getText());
    }
}

void Parser::endLine() {

    match(ENDLNE);

    while(checkToken(ENDLNE)) {
        nextToken();
    }
}
