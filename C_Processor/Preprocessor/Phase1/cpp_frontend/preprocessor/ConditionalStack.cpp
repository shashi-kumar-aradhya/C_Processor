#include "ConditionalStack.h"
#include <iostream>

void ConditionalStack::push(bool active) {
    stack.push(active);
    std::cout << "[TRACE] push(" << active << ")\n";
}

void ConditionalStack::pop() {
    if (!stack.empty()) {
        stack.pop();
        std::cout << "[TRACE] pop()\n";
    }
}

void ConditionalStack::flip() {
    if (!stack.empty()) {
        stack.top() = !stack.top();
        std::cout << "[TRACE] flip() -> " << stack.top() << "\n";
    }
}

bool ConditionalStack::isActive() const {
    if (stack.empty())
        return true;
    return stack.top();
}

