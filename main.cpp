#include <iostream>
#include <fstream>
#include <filesystem>

#include "Emitter/Emitter.h"
#include "Lexer/KeywordIdentifier.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

int main() {
    std::cout << "***** Remarkable Compiler *****" << std::endl;
    std::cout << "Please enter the path to the file you would like to compile!" << std::endl;
    std::string filepath;
    std::cin >> filepath;

    /*Save Keyword Identifier*/
    {
        std::ofstream ofs("../resources/keyword_ident.txt");
        KeywordIdentifier ident;
        ident.init();
        boost::archive::text_oarchive oa(ofs);
        oa << ident;

        ofs.close();
    }


    std::string source;
    std::string readFile(std::filesystem::path path);
    std::ifstream inputFile(filepath, std::ios::in);
    if(inputFile.is_open()) {
        namespace fs = std::filesystem;
        const auto size = fs::file_size(filepath);
        std::string result(size, '\0');
        inputFile.read(&result[0], static_cast<long>(size));
        source = result;
    }
    else {
        std::cerr << "Invalid Filepath: " << filepath << std::endl;
        inputFile.close();
        exit(-1);
    }
    inputFile.close();

    std::string outFile;
    std::cout << "Please enter the output path and filename for your compiled program!" << std::endl;
    std::cin >> outFile;
    if(outFile == "pDir") {
        outFile = "out.c";
    }
    Lexer lex(source);
    Emitter emitter(outFile);
    Parser pars(&lex, &emitter);

    pars.program();
    emitter.writeFile();
    std::cout << "Compiling Completed!" << std::endl;

    return 0;
}
