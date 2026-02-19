#pragma once
#include <vector>
#include "../lexer/Token.h"
#include "MacroTable.h"
#include "ConditionalStack.h"

class Preprocessor {
public:
    Preprocessor(const std::vector<Token>& tokens);

    std::vector<Token> process();

private:
    std::vector<Token> tokens;
    size_t pos = 0;

    MacroTable macros;
    ConditionalStack condStack;

    bool atDirectiveStart(const Token& tok) const;
    void handleDirective();
    void handleDefine();
    void handleUndef();
    void handleIfdef(bool negated);
    void handleIf();
};

