#pragma once
#include <unordered_map>
#include <vector>
#include "../lexer/Token.h"

struct Macro {
    std::vector<Token> replacement;
};

class MacroTable {
public:
    void define(const std::string& name,
                const std::vector<Token>& replacement);

    void undef(const std::string& name);

    bool exists(const std::string& name) const;

    const Macro& get(const std::string& name) const;

private:
    std::unordered_map<std::string, Macro> table;
};

