#pragma once
#include <string>

enum class TokenKind {
    Identifier,
    PPNumber,
    StringLiteral,
    CharLiteral,
    Punctuator,
    HeaderName,
    EndOfFile
};

struct SourceLocation {
    std::string file;
    int line;
    int column;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation loc;

    bool atStartOfLine = false;
    bool leadingSpace = false;
};

