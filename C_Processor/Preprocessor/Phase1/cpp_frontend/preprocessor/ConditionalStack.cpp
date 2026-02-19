#include "ConditionalStack.h"

void ConditionalStack::pushIf(bool condition) {
    bool parentActive = isActive();

    Level level;
    level.parentActive = parentActive;
    level.thisBranchActive = parentActive && condition;
    level.branchTaken = condition;

    stack.push_back(level);
}

void ConditionalStack::pushElse() {
    if (stack.empty())
        return;

    Level& level = stack.back();

    if (!level.parentActive) {
        level.thisBranchActive = false;
    } else {
        level.thisBranchActive = !level.branchTaken;
        level.branchTaken = true;
    }
}

void ConditionalStack::pop() {
    if (!stack.empty())
        stack.pop_back();
}

bool ConditionalStack::isActive() const {
    for (const auto& level : stack)
        if (!level.thisBranchActive)
            return false;

    return true;
}

