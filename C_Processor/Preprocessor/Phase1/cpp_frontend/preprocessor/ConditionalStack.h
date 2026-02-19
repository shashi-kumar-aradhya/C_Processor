#pragma once
#include <stack>

struct ConditionalFrame {
    bool parentActive;     // Outer block active?
    bool branchTaken;      // Has any branch executed?
    bool currentlyActive;  // Is this branch active?
};

class ConditionalStack {
public:
    void pushIf(bool condition, bool parentActive);
    void handleElif(bool condition);
    void handleElse();
    void pop();

    bool isActive() const;

private:
    std::stack<ConditionalFrame> stack;
};

