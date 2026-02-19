#include "MacroTable.h"

void MacroTable::define(const std::string& name,
                        const std::vector<Token>& replacement) {
    table[name] = {replacement};
}

void MacroTable::undef(const std::string& name) {
    table.erase(name);
}

bool MacroTable::exists(const std::string& name) const {
    return table.find(name) != table.end();
}

const Macro& MacroTable::get(const std::string& name) const {
    return table.at(name);
}

