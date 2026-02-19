#include "Preprocessor.h"
#include "../phase1/Phase1Processor.h"
#include "../lexer/Lexer.h"

#include <fstream>
#include <sstream>
#include <iostream>

Preprocessor::Preprocessor(
    const std::vector<Token>& t,
    const std::string& file,
    std::vector<std::string>& stack,
    std::unordered_set<std::string>& guards)
    : tokens(t),
      currentFile(file),
      includeStack(stack),
      includeGuardMacros(guards)
{}

bool Preprocessor::atDirectiveStart(const Token& tok) const {
    return tok.atStartOfLine &&
           tok.kind == TokenKind::Punctuator &&
           tok.text == "#";
}

std::vector<Token> Preprocessor::process() {
    std::vector<Token> output;

    std::string guardMacro;
    if (detectIncludeGuard(guardMacro)) {
        if (includeGuardMacros.count(guardMacro)) {
            return {};
        }
        includeGuardMacros.insert(guardMacro);
    }

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

bool Preprocessor::detectIncludeGuard(std::string& guardMacro) {
    if (tokens.size() < 6)
        return false;

    if (!(tokens[0].atStartOfLine &&
          tokens[0].text == "#" &&
          tokens[1].text == "ifndef"))
        return false;

    guardMacro = tokens[2].text;

    if (!(tokens[3].atStartOfLine &&
          tokens[3].text == "#" &&
          tokens[4].text == "define" &&
          tokens[5].text == guardMacro))
        return false;

    return true;
}

void Preprocessor::handleDirective() {
    if (pos >= tokens.size())
        return;

    std::string directive = tokens[pos].text;
    pos++;

    if (directive == "include")
        handleInclude();
    else if (directive == "ifndef")
        handleIfndef();
    else if (directive == "define")
        handleDefine();
    else if (directive == "else")
        condStack.pushElse();
    else if (directive == "endif")
        condStack.pop();
}

void Preprocessor::handleIfndef() {
    if (pos >= tokens.size())
        return;

    std::string macro = tokens[pos].text;
    pos++;

    bool active = true; // simplified until macro expansion phase
    condStack.pushIf(active);
}

void Preprocessor::handleDefine() {
    if (pos >= tokens.size())
        return;

    std::string macro = tokens[pos].text;
    pos++;

    macros.define(macro, {});
}

void Preprocessor::handleInclude() {
    if (pos >= tokens.size())
        return;

    std::string filename = tokens[pos].text;
    pos++;

    if (filename.front() == '"' || filename.front() == '<')
        filename = filename.substr(1, filename.size() - 2);

    for (const auto& f : includeStack) {
        if (f == filename) {
            std::cerr << "Error: Recursive include detected: "
                      << filename << "\n";
            return;
        }
    }

    includeStack.push_back(filename);

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open include file\n";
        includeStack.pop_back();
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    Phase1Processor p1(buffer.str());
    std::string phase1Out = p1.process();

    // ✅ FIXED: pass filename
    Lexer lexer(phase1Out, filename);
    auto newTokens = lexer.tokenize();

    Preprocessor sub(newTokens,
                     filename,
                     includeStack,
                     includeGuardMacros);

    auto processed = sub.process();

    includeStack.pop_back();

    tokens.insert(tokens.begin() + pos,
                  processed.begin(),
                  processed.end());
}

