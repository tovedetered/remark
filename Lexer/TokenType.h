//
// Created by tobedetered on 2/17/24.
//
#pragma once
#ifndef TOKENTYPE_H
#define TOKENTYPE_H

enum TokenType {
    eof = - 1,
    newline = 0,
    number = 1,
    ident = 2,
    strlit = 3,

    //----- Keywords -----\\

    LBL = 101,
    GOTO = 102,

    PRNT = 103,
    INPT = 104,

    LET = 105,

    IF = 106,
    THEN = 107,
    ELSE = 114,
    ELSEIF = 115,

    BEGIN = 108,
    END = 109,
    STRTMOD = 110,
    ENDMOD = 111,
    ENDLNE = 112,
    ENDIF = 113,

    WHILE = 120,
    REPEAT = 121,
    ENDWHILE = 122,

    //----- Types -----\\

    INT = 201,
    CHAR = 202,
    STR = 203,
    FLOAT = 204,

    //----- Operators -----\\

    EQ = 301,
    PLUS = 302,
    MINUS = 303,
    ASTERISK = 304,
    SLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211,
};

#endif //TOKENTYPE_H