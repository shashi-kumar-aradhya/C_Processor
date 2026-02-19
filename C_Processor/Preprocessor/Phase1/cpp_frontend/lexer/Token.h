#pragma once
#include <string>

enum class TokenKind {
    Identifier,
    PPNumber,
    StringLiteral,
    CharLiteral,
    Punctuator,
    EndOfFile
};

struct Token {
    TokenKind kind;
    std::string text;
    bool atStartOfLine = false;
};

