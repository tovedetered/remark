cmake_minimum_required(VERSION 3.22.0)
project(remark)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.74.0 COMPONENTS serialization filesystem)

if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        add_executable(remark
                main.cpp
                Lexer/KeywordIdentifier.h
                Lexer/Lexer.cpp
                Lexer/Lexer.h
                Lexer/Token.cpp
                Lexer/Token.h
                Lexer/TokenType.h
                Parser/Parser.cpp
                Parser/Parser.h
                Emitter/Emitter.cpp
                Emitter/Emitter.h
        )
        target_link_libraries(remark ${Boost_LIBRARIES})
endif()




