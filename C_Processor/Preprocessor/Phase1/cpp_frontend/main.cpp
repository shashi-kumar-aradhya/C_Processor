#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "phase1/Phase1Processor.h"
#include "lexer/Lexer.h"
#include "preprocessor/Preprocessor.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: cpp_frontend <file.c>\n";
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    Phase1Processor p1(buffer.str());
    std::string phase1Out = p1.process();

    Lexer lexer(phase1Out, filename);
    auto tokens = lexer.tokenize();

    std::vector<std::string> includeStack;
    includeStack.push_back(filename);

    std::unordered_set<std::string> includeGuardMacros;

    Preprocessor pp(tokens,
                    filename,
                    includeStack,
                    includeGuardMacros);

    auto result = pp.process();

    for (auto& t : result)
        std::cout << t.text << "\n";

    return 0;
}

