#pragma once
#include <vector>
#include <iostream>
#include <exception>

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
    char symbol_;
    GrammarSymbol() {}
    GrammarSymbol(char symbol) : symbol_(symbol){}
    virtual bool IsTerminal ()const {
        return false;
    };

    bool operator == (const GrammarSymbol& other) const {
        return symbol_ == other.symbol_;
    }
    bool operator < (const GrammarSymbol&other) const {
        return symbol_ < other.symbol_;
    }
};

struct Terminal : public GrammarSymbol {
    Terminal() {}
    Terminal(char terminal) : GrammarSymbol(terminal){};
    bool IsTerminal()const override final {
        return true;
    }

    bool operator == (const Terminal& other)const {
        return symbol_ == other.symbol_;
    }
    bool operator == (const GrammarSymbol& other) const {
        return other.IsTerminal() && *this == other;
    }
};


struct NeTerminal : public GrammarSymbol {
    NeTerminal() {}
    NeTerminal(char neTerminal) : GrammarSymbol(neTerminal){};
    bool IsTerminal()const override final {
        return false;
    }

    bool operator == (const NeTerminal& other) const {
        return symbol_ == other.symbol_;
    }
    bool operator == (const GrammarSymbol& other) const {
        return !other.IsTerminal() && *this == other;
    }
};

static const Terminal Epsilon = Terminal (' ');
struct Rule {
    std::pair<NeTerminal, std::vector<GrammarSymbol>> rule_;
    NeTerminal left_;
    std::vector<GrammarSymbol> right_;

    Rule(NeTerminal left, std::vector<GrammarSymbol> right) : left_(left), right_(right) {
        if (!right_.size()) {
            right_.push_back (Epsilon);
        }
        rule_ = std::pair<NeTerminal, std::vector<GrammarSymbol>>(left_, right_);
    }
    Rule(NeTerminal left, std::initializer_list<GrammarSymbol> list) : left_(left) {
        for (auto elem: list) {
            right_.push_back(elem);
        };
        if (!right_.size()) {
            right_.push_back (Epsilon);
        }
        rule_ = std::pair<NeTerminal, std::vector<GrammarSymbol>>(left_, right_);
    }

    void Print() {
        std::cout << left_.symbol_ << " ->";
        for (auto& elem: right_) {
            std::cout << " " << elem.symbol_;
        }
        std::cout << std::endl;
    }
};

struct Grammar { //
    NeTerminal start_;
    std::vector<Rule> rules_;
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
    bool IsTerminal (char symbol) {
        Terminal t (symbol);
        for (auto elem: alphabet_)
            if (elem == t) 
                return true;
        return false;
    }
    bool IsNeTerminal (char symbol) {
        NeTerminal t (symbol);
        for (auto elem: neTerminals_)
            if (elem == t) 
                return true;
        return false;
    }
};