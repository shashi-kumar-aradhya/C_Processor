#pragma once
#include <stack>

class ConditionalStack {
public:
    void push(bool active);
    void pop();
    void flip();
    bool isActive() const;

private:
    std::stack<bool> stack;
};

