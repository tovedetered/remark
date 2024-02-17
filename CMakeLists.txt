cmake_minimum_required(VERSION 3.28)
project(remark)

set(CMAKE_CXX_STANDARD 20)

add_executable(remark main.cpp
        Lexer/Lexer.cpp
        Lexer/Lexer.h
        Lexer/Token.cpp
        Lexer/Token.h
        Lexer/TokenType.h)
