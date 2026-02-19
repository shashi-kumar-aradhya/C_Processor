#include "ExpressionParser.h"
#include <cstdlib>
#include <stdexcept>

ExpressionParser::ExpressionParser(
    const std::vector<Token>& t,
    MacroTable& m)
    : tokens(t), macros(m) {}

Token ExpressionParser::peek() const {
    if (pos >= tokens.size())
        return tokens.back();
    return tokens[pos];
}

bool ExpressionParser::match(const std::string& op) {
    if (peek().text == op) {
        pos++;
        return true;
    }
    return false;
}

long long ExpressionParser::evaluate() {
    return parseExpression();
}

long long ExpressionParser::parseExpression() {
    return parseLogicalOr();
}

long long ExpressionParser::parseLogicalOr() {
    long long lhs = parseLogicalAnd();
    while (match("||")) {
        long long rhs = parseLogicalAnd();
        lhs = (lhs || rhs);
    }
    return lhs;
}

long long ExpressionParser::parseLogicalAnd() {
    long long lhs = parseBitwiseOr();
    while (match("&&")) {
        long long rhs = parseBitwiseOr();
        lhs = (lhs && rhs);
    }
    return lhs;
}

long long ExpressionParser::parseBitwiseOr() {
    long long lhs = parseBitwiseXor();
    while (match("|")) {
        long long rhs = parseBitwiseXor();
        lhs = lhs | rhs;
    }
    return lhs;
}

long long ExpressionParser::parseBitwiseXor() {
    long long lhs = parseBitwiseAnd();
    while (match("^")) {
        long long rhs = parseBitwiseAnd();
        lhs = lhs ^ rhs;
    }
    return lhs;
}

long long ExpressionParser::parseBitwiseAnd() {
    long long lhs = parseEquality();
    while (match("&")) {
        long long rhs = parseEquality();
        lhs = lhs & rhs;
    }
    return lhs;
}

long long ExpressionParser::parseEquality() {
    long long lhs = parseRelational();
    while (true) {
        if (match("=="))
            lhs = (lhs == parseRelational());
        else if (match("!="))
            lhs = (lhs != parseRelational());
        else
            break;
    }
    return lhs;
}

long long ExpressionParser::parseRelational() {
    long long lhs = parseShift();
    while (true) {
        if (match("<"))
            lhs = (lhs < parseShift());
        else if (match(">"))
            lhs = (lhs > parseShift());
        else if (match("<="))
            lhs = (lhs <= parseShift());
        else if (match(">="))
            lhs = (lhs >= parseShift());
        else
            break;
    }
    return lhs;
}

long long ExpressionParser::parseShift() {
    long long lhs = parseAdditive();
    while (true) {
        if (match("<<"))
            lhs = lhs << parseAdditive();
        else if (match(">>"))
            lhs = lhs >> parseAdditive();
        else
            break;
    }
    return lhs;
}

long long ExpressionParser::parseAdditive() {
    long long lhs = parseMultiplicative();
    while (true) {
        if (match("+"))
            lhs = lhs + parseMultiplicative();
        else if (match("-"))
            lhs = lhs - parseMultiplicative();
        else
            break;
    }
    return lhs;
}

long long ExpressionParser::parseMultiplicative() {
    long long lhs = parseUnary();
    while (true) {
        if (match("*"))
            lhs = lhs * parseUnary();
        else if (match("/"))
            lhs = lhs / parseUnary();
        else if (match("%"))
            lhs = lhs % parseUnary();
        else
            break;
    }
    return lhs;
}

long long ExpressionParser::parseUnary() {
    if (match("!"))
        return !parseUnary();
    if (match("-"))
        return -parseUnary();
    if (match("+"))
        return parseUnary();

    if (peek().text == "defined") {
        pos++;
        if (match("(")) {
            std::string name = peek().text;
            pos++;
            match(")");
            return macros.exists(name) ? 1 : 0;
        } else {
            std::string name = peek().text;
            pos++;
            return macros.exists(name) ? 1 : 0;
        }
    }

    return parsePrimary();
}

long long ExpressionParser::parsePrimary() {
    if (match("(")) {
        long long value = parseExpression();
        match(")");
        return value;
    }

    Token t = peek();
    pos++;

    if (t.kind == TokenKind::PPNumber)
        return std::atoll(t.text.c_str());

    if (t.kind == TokenKind::Identifier)
        return 0; // undefined identifiers → 0

    return 0;
}

