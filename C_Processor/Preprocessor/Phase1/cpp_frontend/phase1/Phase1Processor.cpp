#include "Phase1Processor.h"

Phase1Processor::Phase1Processor(const std::string& input)
    : source(input) {}

std::string Phase1Processor::process() {
    std::string step1 = normalizeNewlines(source);
    std::string step2 = replaceTrigraphs(step1);
    std::string step3 = spliceLines(step2);
    return step3;
}

// Convert CRLF and CR to LF
std::string Phase1Processor::normalizeNewlines(const std::string& in) {
    std::string out;
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '\r') {
            if (i + 1 < in.size() && in[i + 1] == '\n')
                i++;
            out += '\n';
        } else {
            out += in[i];
        }
    }
    return out;
}

// Replace C trigraph sequences
std::string Phase1Processor::replaceTrigraphs(const std::string& in) {
    std::string out;
    for (size_t i = 0; i < in.size(); ++i) {
        if (i + 2 < in.size() && in[i] == '?' && in[i + 1] == '?') {
            char repl = 0;
            switch (in[i + 2]) {
                case '=': repl = '#'; break;
                case '/': repl = '\\'; break;
                case '\'': repl = '^'; break;
                case '(': repl = '['; break;
                case ')': repl = ']'; break;
                case '!': repl = '|'; break;
                case '<': repl = '{'; break;
                case '>': repl = '}'; break;
                case '-': repl = '~'; break;
            }
            if (repl) {
                out += repl;
                i += 2;
                continue;
            }
        }
        out += in[i];
    }
    return out;
}

// Remove backslash-newline pairs
std::string Phase1Processor::spliceLines(const std::string& in) {
    std::string out;
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '\\' && i + 1 < in.size() && in[i + 1] == '\n') {
            i++; // skip newline
            continue;
        }
        out += in[i];
    }
    return out;
}
