#include <iostream>
#include <fstream>
#include <sstream>
#include "phase1/Phase1Processor.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: cpp_frontend <file.c>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    Phase1Processor p(buffer.str());
    std::string result = p.process();

    std::cout << result;
}

