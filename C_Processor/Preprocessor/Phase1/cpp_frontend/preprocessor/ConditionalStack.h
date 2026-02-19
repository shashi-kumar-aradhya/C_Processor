#pragma once
#include <vector>

class ConditionalStack {
public:
    void pushIf(bool condition);
    void pushElse();
    void pop();
    bool isActive() const;

private:
    struct Level {
        bool parentActive;
        bool thisBranchActive;
        bool branchTaken;
    };

    std::vector<Level> stack;
};

