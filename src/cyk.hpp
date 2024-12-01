#pragma once

#include "grammar/grammar.hpp"

class CYK {
    Grammar *grammar_ = nullptr;
public:
    void fit(Grammar& grammar) {
        grammar.ConvertToChomsky();
        grammar_ = &grammar; 
    }
    bool predict(const std::string& string) {
        return predict(string.c_str());
    }
    bool predict(const char *string);
};