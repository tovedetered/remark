#include <complex>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Dependencies/cxxopts.hpp"

#include "Emitter/Emitter.h"
#include "Lexer/KeywordIdentifier.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

int main(int argc, char** argv) {

    cxxopts::Options options("Remark", "A compiler for the Remark Programming Language");

    options.add_options()
        ("o,output_name", "Output Name", cxxopts::value<std::string>())
        ("f,file", "File To Compile", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    std::string filepath;
    if(result.count("file")) {
        filepath = result["file"].as<std::string>();
    }
    else {
        std::cerr << "ERROR: No filepath inputted! please use -f {file-path}" << std::endl;
    }
    std::string outFile;
    if(result.count("output_name")) {
        outFile = result["output_name"].as<std::string>();
    }
    else {
        outFile = "out.c";
    }

    std::cout << "***** Remarkable Compiler *****" << std::endl;

    /*Save Keyword Identifier
    {
        std::ofstream ofs("../resources/keyword_ident.txt");
        KeywordIdentifier ident;
        ident.init();
        boost::archive::text_oarchive oa(ofs);
        oa << ident;

        ofs.close();
    }
    */


    std::string source;
    std::string readFile(std::filesystem::path path);
    std::ifstream inputFile(filepath, std::ios::in);
    if(inputFile.is_open()) {
        namespace fs = std::filesystem;
        const auto size = fs::file_size(filepath);
        std::string outcome(size, '\0');
        inputFile.read(&outcome[0], static_cast<long>(size));
        source = outcome;
    }
    else {
        std::cerr << "Invalid Filepath: " << filepath << std::endl;
        inputFile.close();
        exit(-1);
    }
    inputFile.close();


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
