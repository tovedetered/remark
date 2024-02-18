#include <iostream>
#include <fstream>

#include "Lexer/KeywordIdentifier.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

int main() {
    std::cout << "***** Remarkable Compiler *****" << std::endl;
    std::string filepath;
    std::cin >> filepath;

    std::ifstream inputFile(filepath);

    /*Save Keyword Identifier
    {
        std::ofstream ofs("/home/tobedetered/CLionProjects/remark/resources/keyword_ident.txt");
        KeywordIdentifier ident;
        ident.init();
        boost::archive::text_oarchive oa(ofs);
        oa << ident;

        ofs.close();
    }
    */

    return 0;
}
