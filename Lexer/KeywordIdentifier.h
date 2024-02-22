//
// Created by tobedetered on 2/18/24.
//

#ifndef KEYWORDIDENTIFIER_H
#define KEYWORDIDENTIFIER_H
#include <string>
#include <unordered_map>

#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>

#include "TokenType.h"

class KeywordIdentifier {
public:
    KeywordIdentifier() = default;
    TokenType getKeyword(const std::string& text) const;
    void init();
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & keywordMap;
    }
    void deserialize();
private:
    friend class boost::serialization::access;
    std::unordered_map<std::string, TokenType> keywordMap;
};

inline TokenType KeywordIdentifier::getKeyword(const std::string& text) const {
    if(keywordMap.contains(text)) {
        return keywordMap.at(text);
    }
    return ident;
}

inline void KeywordIdentifier::init() {
    // Initialze keywords
    keywordMap.emplace("lbl", LBL);
    keywordMap.emplace("goto", GOTO);
    keywordMap.emplace("prnt", PRNT);
    keywordMap.emplace("print", PRNT);
    keywordMap.emplace("inpt", INPT);
    keywordMap.emplace("input", INPT);
    keywordMap.emplace("let", LET);
    keywordMap.emplace("if", IF);
    keywordMap.emplace("then", THEN);
    keywordMap.emplace("begin", BEGIN);
    keywordMap.emplace("end", END);
    keywordMap.emplace("strtmod", STRTMOD);
    keywordMap.emplace("endmod", ENDMOD);
    keywordMap.emplace("while", WHILE);
    keywordMap.emplace("repeat", REPEAT);
    keywordMap.emplace(";", ENDLNE);
    keywordMap.emplace("!", ENDLNE);

    // Initialize Types
    keywordMap.emplace("int", INT);
    keywordMap.emplace("char", CHAR);
    keywordMap.emplace("str", STR);
    keywordMap.emplace("float", FLOAT);
}
#endif //KEYWORDIDENTIFIER_H
