#pragma once
#include <vector>
#include <string>
#include <unordered_set>

#include "../lexer/Token.h"
#include "MacroTable.h"
#include "ConditionalStack.h"

class Preprocessor {
public:
    Preprocessor(const std::vector<Token>& tokens,
                 const std::string& currentFile,
                 std::vector<std::string>& includeStack,
                 std::unordered_set<std::string>& includeGuardMacros);

    std::vector<Token> process();

private:
    std::vector<Token> tokens;
    size_t pos = 0;

    std::string currentFile;
    std::vector<std::string>& includeStack;
    std::unordered_set<std::string>& includeGuardMacros;

    MacroTable macros;
    ConditionalStack condStack;

    bool atDirectiveStart(const Token& tok) const;

    void handleDirective();
    void handleInclude();
    void handleIfndef();
    void handleDefine();

    bool detectIncludeGuard(std::string& guardMacro);
};

