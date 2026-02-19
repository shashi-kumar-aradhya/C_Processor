#pragma once
#include <vector>
#include "../lexer/Token.h"
#include "MacroTable.h"

class ExpressionParser {
public:
    ExpressionParser(const std::vector<Token>& tokens,
                     MacroTable& macros);

    long long evaluate();

private:
    const std::vector<Token>& tokens;
    MacroTable& macros;
    size_t pos = 0;

    long long parseExpression();
    long long parseLogicalOr();
    long long parseLogicalAnd();
    long long parseBitwiseOr();
    long long parseBitwiseXor();
    long long parseBitwiseAnd();
    long long parseEquality();
    long long parseRelational();
    long long parseShift();
    long long parseAdditive();
    long long parseMultiplicative();
    long long parseUnary();
    long long parsePrimary();

    bool match(const std::string& op);
    Token peek() const;
};

