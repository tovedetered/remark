//
// Created by tobedetered on 2/21/24.
//

#include "Emitter.h"

#include <iostream>

Emitter::Emitter(const std::string& outputPath) {
    this->outputPath = outputPath;
    header = "";
    code = "";
}

void Emitter::writeFile() const {
    std::ofstream out(outputPath);
    if(!out.is_open()) {
        std::cerr << "EMITTER ERROR: Invalid File Path" << std::endl;
        exit(-1);
    }
    out << header << code;
    out.close();
}

void Emitter::emit(const std::string& code) {
    this->code += code;
}

void Emitter::emitLine(const std::string& code) {
    this->code += code + '\n';
}

void Emitter::headerLine(const std::string& code) {
    this->header += code + '\n';
}
