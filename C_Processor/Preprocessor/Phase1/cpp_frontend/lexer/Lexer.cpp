#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input,
             const std::string& filename)
    : source(input), file(filename) {}

char Lexer::peek(int offset) const {
    if (pos + offset >= source.size())
        return '\0';
    return source[pos + offset];
}

char Lexer::get() {
    if (eof()) return '\0';

    char c = source[pos++];
    if (c == '\n') {
        line++;
        column = 1;
        atStartOfLine = true;
    } else {
        column++;
        atStartOfLine = false;
    }
    return c;
}

bool Lexer::eof() const {
    return pos >= source.size();
}

bool Lexer::isIdentifierStart(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isIdentifierChar(char c) const {
    return std::isalnum(c) || c == '_';
}

void Lexer::skipWhitespace(bool& leadingSpace) {
    while (std::isspace(peek())) {
        leadingSpace = true;
        get();
    }
}

void Lexer::skipComment(bool& leadingSpace) {
    if (peek() == '/' && peek(1) == '/') {
        leadingSpace = true;
        while (!eof() && get() != '\n');
    }
    else if (peek() == '/' && peek(1) == '*') {
        leadingSpace = true;
        get(); get();
        while (!eof()) {
            if (peek() == '*' && peek(1) == '/') {
                get(); get();
                break;
            }
            get();
        }
    }
}

Token Lexer::lexIdentifier(bool leadingSpace) {
    size_t startCol = column;
    std::string text;

    while (isIdentifierChar(peek()))
        text += get();

    return {
        TokenKind::Identifier,
        text,
        {file, line, (int)startCol},
        atStartOfLine,
        leadingSpace
    };
}

Token Lexer::lexNumber(bool leadingSpace) {
    size_t startCol = column;
    std::string text;

    while (std::isalnum(peek()) ||
           peek() == '.' ||
           peek() == '+' ||
           peek() == '-' )
    {
        text += get();
    }

    return {
        TokenKind::PPNumber,
        text,
        {file, line, (int)startCol},
        atStartOfLine,
        leadingSpace
    };
}

Token Lexer::lexString(bool leadingSpace) {
    size_t startCol = column;
    std::string text;
    text += get(); // opening "

    while (!eof()) {
        char c = get();
        text += c;
        if (c == '\\') {
            text += get();
        }
        else if (c == '"')
            break;
    }

    return {
        TokenKind::StringLiteral,
        text,
        {file, line, (int)startCol},
        atStartOfLine,
        leadingSpace
    };
}

Token Lexer::lexChar(bool leadingSpace) {
    size_t startCol = column;
    std::string text;
    text += get(); // '

    while (!eof()) {
        char c = get();
        text += c;
        if (c == '\\') {
            text += get();
        }
        else if (c == '\'')
            break;
    }

    return {
        TokenKind::CharLiteral,
        text,
        {file, line, (int)startCol},
        atStartOfLine,
        leadingSpace
    };
}

Token Lexer::lexPunctuator(bool leadingSpace) {
    size_t startCol = column;

    static const std::vector<std::string> multi = {
        ">>=", "<<=", "...", "->",
        "++", "--", "&&", "||",
        "<=", ">=", "==", "!=",
        "<<", ">>", "+=", "-=",
        "*=", "/=", "%=", "&=",
        "^=", "|=", "##"
    };

    for (const auto& m : multi) {
        if (source.substr(pos, m.size()) == m) {
            for (size_t i = 0; i < m.size(); ++i)
                get();
            return {
                TokenKind::Punctuator,
                m,
                {file, line, (int)startCol},
                atStartOfLine,
                leadingSpace
            };
        }
    }

    std::string single(1, get());

    return {
        TokenKind::Punctuator,
        single,
        {file, line, (int)startCol},
        atStartOfLine,
        leadingSpace
    };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!eof()) {
        bool leadingSpace = false;

        skipWhitespace(leadingSpace);
        skipComment(leadingSpace);

        if (eof()) break;

        if (isIdentifierStart(peek()))
            tokens.push_back(lexIdentifier(leadingSpace));
        else if (std::isdigit(peek()) ||
                 (peek() == '.' && std::isdigit(peek(1))))
            tokens.push_back(lexNumber(leadingSpace));
        else if (peek() == '"')
            tokens.push_back(lexString(leadingSpace));
        else if (peek() == '\'')
            tokens.push_back(lexChar(leadingSpace));
        else
            tokens.push_back(lexPunctuator(leadingSpace));
    }

    tokens.push_back({
        TokenKind::EndOfFile,
        "",
        {file, line, column},
        false,
        false
    });

    return tokens;
}

