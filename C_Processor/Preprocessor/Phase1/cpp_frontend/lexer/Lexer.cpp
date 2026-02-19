#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& in,
             const std::string& file)
    : input(in), filename(file) {}

char Lexer::peek() const {
    if (pos >= input.size()) return '\0';
    return input[pos];
}

char Lexer::get() {
    if (pos >= input.size()) return '\0';
    return input[pos++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) {
        if (peek() == '\n')
            startOfLine = true;
        get();
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < input.size()) {
        skipWhitespace();

        if (pos >= input.size())
            break;

        Token tok;
        tok.atStartOfLine = startOfLine;
        startOfLine = false;

        char c = peek();

        // Identifier
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::string id;
            while (std::isalnum(static_cast<unsigned char>(peek())) ||
                   peek() == '_')
                id += get();

            tok.kind = TokenKind::Identifier;
            tok.text = id;
        }
        // PP Number
        else if (std::isdigit(static_cast<unsigned char>(c))) {
            std::string num;
            while (std::isalnum(static_cast<unsigned char>(peek())) ||
                   peek() == '.')
                num += get();

            tok.kind = TokenKind::PPNumber;
            tok.text = num;
        }
        // 🔥 String literal (FIX)
        else if (c == '"') {
            std::string str;
            str += get(); // opening quote

            while (peek() != '"' && peek() != '\0') {
                if (peek() == '\\') { // handle escape
                    str += get();
                    if (peek() != '\0')
                        str += get();
                } else {
                    str += get();
                }
            }

            if (peek() == '"')
                str += get(); // closing quote

            tok.kind = TokenKind::StringLiteral;
            tok.text = str;
        }
        else {
            tok.kind = TokenKind::Punctuator;
            tok.text = std::string(1, get());
        }

        tokens.push_back(tok);
    }

    return tokens;
}

