#pragma once
#include <string>
#include <vector>
#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& input,
          const std::string& filename);

    std::vector<Token> tokenize();

private:
    std::string input;
    std::string filename;
    size_t pos = 0;
    bool startOfLine = true;

    char peek() const;
    char get();
    void skipWhitespace();
};

