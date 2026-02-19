#include "Preprocessor.h"
#include "ExpressionParser.h"
#include <iostream>

Preprocessor::Preprocessor(const std::vector<Token>& t)
    : tokens(t) {}

bool Preprocessor::atDirectiveStart(const Token& tok) const {
    return tok.atStartOfLine &&
           tok.kind == TokenKind::Punctuator &&
           tok.text == "#";
}

std::vector<Token> Preprocessor::process() {
    std::vector<Token> output;

    while (pos < tokens.size()) {
        Token& tok = tokens[pos];

        if (atDirectiveStart(tok)) {
            pos++;
            handleDirective();
            continue;
        }

        if (condStack.isActive())
            output.push_back(tok);

        pos++;
    }

    return output;
}

void Preprocessor::handleDirective() {
    if (pos >= tokens.size()) return;

    Token directive = tokens[pos++];

    if (directive.text == "define")
        handleDefine();
    else if (directive.text == "undef")
        handleUndef();
    else if (directive.text == "ifdef")
        handleIfdef(false);
    else if (directive.text == "ifndef")
        handleIfdef(true);
    else if (directive.text == "if")
        handleIf();
    else if (directive.text == "elif")
        handleElif();
    else if (directive.text == "else")
        condStack.handleElse();
    else if (directive.text == "endif")
        condStack.pop();
}

void Preprocessor::handleDefine() {
    if (!condStack.isActive())
        return;

    Token name = tokens[pos++];

    std::vector<Token> replacement;

    while (pos < tokens.size() &&
           !tokens[pos].atStartOfLine)
    {
        replacement.push_back(tokens[pos++]);
    }

    macros.define(name.text, replacement);
}

void Preprocessor::handleUndef() {
    if (!condStack.isActive())
        return;

    Token name = tokens[pos++];
    macros.undef(name.text);
}

void Preprocessor::handleIfdef(bool negated) {
    Token name = tokens[pos++];

    bool exists = macros.exists(name.text);
    bool result = negated ? !exists : exists;

    condStack.pushIf(result, condStack.isActive());
}

void Preprocessor::handleIf() {
    std::vector<Token> exprTokens;

    while (pos < tokens.size() &&
           !tokens[pos].atStartOfLine)
    {
        exprTokens.push_back(tokens[pos++]);
    }

    ExpressionParser parser(exprTokens, macros);
    long long result = parser.evaluate();

    condStack.pushIf(result != 0, condStack.isActive());
}

void Preprocessor::handleElif() {
    std::vector<Token> exprTokens;

    while (pos < tokens.size() &&
           !tokens[pos].atStartOfLine)
    {
        exprTokens.push_back(tokens[pos++]);
    }

    ExpressionParser parser(exprTokens, macros);
    long long result = parser.evaluate();

    condStack.handleElif(result != 0);
}

