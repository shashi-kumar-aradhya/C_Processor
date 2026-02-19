#include "ConditionalStack.h"

void ConditionalStack::pushIf(bool condition, bool parentActive) {
    ConditionalFrame frame;
    frame.parentActive = parentActive;
    frame.branchTaken = condition && parentActive;
    frame.currentlyActive = condition && parentActive;
    stack.push(frame);
}

void ConditionalStack::handleElif(bool condition) {
    if (stack.empty()) return;

    auto& frame = stack.top();

    if (!frame.parentActive) {
        frame.currentlyActive = false;
        return;
    }

    if (frame.branchTaken) {
        frame.currentlyActive = false;
    } else {
        frame.currentlyActive = condition;
        if (condition)
            frame.branchTaken = true;
    }
}

void ConditionalStack::handleElse() {
    if (stack.empty()) return;

    auto& frame = stack.top();

    if (!frame.parentActive) {
        frame.currentlyActive = false;
        return;
    }

    if (frame.branchTaken) {
        frame.currentlyActive = false;
    } else {
        frame.currentlyActive = true;
        frame.branchTaken = true;
    }
}

void ConditionalStack::pop() {
    if (!stack.empty())
        stack.pop();
}

bool ConditionalStack::isActive() const {
    if (stack.empty())
        return true;
    return stack.top().currentlyActive;
}

