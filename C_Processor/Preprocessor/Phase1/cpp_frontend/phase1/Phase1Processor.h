#pragma once
#include <string>

class Phase1Processor {
public:
    Phase1Processor(const std::string& input);

    std::string process();

private:
    std::string source;

    std::string normalizeNewlines(const std::string& in);
    std::string replaceTrigraphs(const std::string& in);
    std::string spliceLines(const std::string& in);
};
