#pragma once

#include "grammar/grammar.hpp"
struct GrammarSymbolHasher {
    size_t operator() (const GrammarSymbol& grammarSymbol)const {
        return std::hash<char>{}(grammarSymbol.symbol_);
    }
};

class CYK {
    Grammar *grammar_ = nullptr;
    size_t neTerminalsSize_ = 0;
    std::unordered_map<int, NeTerminal> map_;
    std::unordered_map<GrammarSymbol, int, GrammarSymbolHasher> reverseMap_;
public:
    void fit(Grammar& grammar) {
        map_.clear();
        reverseMap_.clear();
        grammar.ConvertToChomsky();
        grammar_ = &grammar; 
        neTerminalsSize_ = grammar.neTerminalsSize_;
        size_t i = 0;
        for (auto& neTerminal: grammar.neTerminals_) {
            reverseMap_[neTerminal] = i;
            map_[i++] = neTerminal; 
        }
    }
    bool predict(const std::string& string) {
        return predict(string.c_str());
    }
    bool predict(const char *string);
};