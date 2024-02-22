//
// Created by tobedetered on 2/21/24.
//

#ifndef EMITTER_H
#define EMITTER_H

#include <string>
#include <fstream>


class Emitter {
public:
    explicit Emitter(const std::string& outputPath);

    void writeFile() const;
    void emit(const std::string& code);
    void emitLine(const std::string& code);
    void headerLine(const std::string& code);

private:
    std::string outputPath;
    std::string header;
    std::string code;
};



#endif //EMITTER_H
