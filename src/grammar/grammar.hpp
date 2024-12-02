#pragma once
#include <exception>
#include <iostream>
#include <vector>
#include <unordered_map>


enum class GrammarType {
    Any, Chomsky, LR0, LR1, LRk
};
// enum class RuleType {}

class GrammarException : public std::exception
{
    std::string msg_;
public:
    GrammarException(const char * message) : msg_(message) {}
    const char* what() const throw()
    {
        return msg_.c_str();
    }
};

struct GrammarSymbol {
    size_t symbol_ = 0;
    bool isTerminal_ = false;
 
    GrammarSymbol() = default;
    GrammarSymbol(size_t symbol, bool isTerminal = false) : 
        symbol_(symbol), isTerminal_(isTerminal){}
    GrammarSymbol(char symbol, bool isTerminal = false) : 
        GrammarSymbol(static_cast<size_t>(symbol), isTerminal) {}

    bool IsTerminal ()const {
        return isTerminal_;
    };

    bool operator == (const GrammarSymbol& other) const {
        return isTerminal_ == other.isTerminal_ && symbol_ == other.symbol_;
    }
    bool operator < (const GrammarSymbol&other) const {
        return symbol_ < other.symbol_;
    }
};

struct Terminal : public GrammarSymbol {
    Terminal() = default;
    Terminal(size_t terminal) : GrammarSymbol(terminal, true){ 
        if (terminal > 256) {
            throw GrammarException("Terminals must be characters");
    }};
    Terminal(char terminal) : GrammarSymbol(terminal, true){};
};

struct NeTerminal : public GrammarSymbol {
    NeTerminal() = default;
    NeTerminal(size_t neTerminal) : GrammarSymbol(neTerminal){};
    NeTerminal(char neTerminal) : GrammarSymbol(neTerminal){};
};

static const Terminal Epsilon = Terminal (' ');

struct Rule {
    NeTerminal left_;
    std::vector<GrammarSymbol> right_;

    Rule () {}
    Rule(NeTerminal left, std::vector<GrammarSymbol> right) : left_(left), right_(right) {
        if (!right_.size()) {
            right_.push_back(Epsilon);
        }
    }
    Rule(NeTerminal left, std::initializer_list<GrammarSymbol> list) : left_(left) {
        for (auto elem: list) {
            right_.push_back(elem);
        };
        if (!right_.size()) {
            right_.push_back(Epsilon);
        }
    }

    void Print() {
        std::cout << static_cast<char>(left_.symbol_) << " ->";
        for (auto elem: right_) {
            if (!elem.isTerminal_ && elem.symbol_ > 256) {
                std::cout << " " << elem.symbol_;
            } else {
                std::cout << " " << static_cast<char>(elem.symbol_);
            }
        }
        std::cout << std::endl;
    }
};

struct NeTerminalHasher {
    size_t operator() (const NeTerminal& neTerminal)const {
        return std::hash<size_t>{}(neTerminal.symbol_);
    }
};

struct Grammar { //
    std::unordered_map<NeTerminal, std::vector<Rule>,NeTerminalHasher> rules__ = {};
    NeTerminal start_;
    // std::vector<Rule> rules_;
    std::vector<Terminal> alphabet_;    
    std::vector<NeTerminal> neTerminals_;
    size_t rulesSize_ = 0;
    size_t alphabetSize_ = 0;
    size_t neTerminalsSize_ = 0;
    GrammarType type_ = GrammarType::Any;

public:
    Grammar() {}
    void CreateFromStdin();    
    GrammarType getType() {
        return type_;
    }
    void ConvertToChomsky();

    void Print();
private:
    void ReadRuleFromStdin();
    bool IsTerminal(char symbol) {
        Terminal t (symbol);
        for (auto elem: alphabet_)
            if (elem == t) 
                return true;
        return false;
    }
    bool IsNeTerminal(char symbol) {
        NeTerminal t (symbol);
        for (auto elem: neTerminals_)
            if (elem == t) 
                return true;
        return false;
    }
    void RemoveLongRules();
    void RemoveLongRule(std::vector<Rule>& new_rules, Rule& rule);
};