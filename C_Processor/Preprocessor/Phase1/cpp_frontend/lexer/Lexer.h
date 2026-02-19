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
    std::string source;
    std::string file;
    size_t pos = 0;
    int line = 1;
    int column = 1;

    // TRUE only at logical beginning of line
    bool atStartOfLine = true;

    char peek(int offset = 0) const;
    char get();
    bool eof() const;

    bool isIdentifierStart(char c) const;
    bool isIdentifierChar(char c) const;

    void skipHorizontalWhitespace(bool& leadingSpace);
    void skipComment(bool& leadingSpace);

    Token lexIdentifier(bool leadingSpace);
    Token lexNumber(bool leadingSpace);
    Token lexString(bool leadingSpace);
    Token lexChar(bool leadingSpace);
    Token lexPunctuator(bool leadingSpace);
};

