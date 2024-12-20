
#include <iostream>
#include <sstream>

#include "grammar/grammar.hpp"
#include "cyk.hpp"

void runAlgo ();

int main () {
    runAlgo ();
}

void runAlgo () {
    Grammar grammar;
    grammar.CreateFromStdin();
    /// grammar.Print();

    grammar.ConvertToChomsky();
    /// grammar.Print();

    CYK cyk;
    cyk.fit(grammar);

    size_t N; 
    std::cin >> N;

    std::vector<std::string> stringsToPredict;
    for (size_t i = 0; i < N; ++i) {
        std::string str;
        std::cin >> str;
        stringsToPredict.push_back(str);
    }

    bool res;
    for (size_t i = 0; i < N; ++i) {
        res = cyk.predict(stringsToPredict[i]);
        //std::cout << "res for " << stringsToPredict[i] << " = " << res << "\n";
        if (res) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
}

/* stdin example
3 2 5
SBC
ab
S->BC
B->BB
C->CC
C->b
B->a
S
5
ab
abb
aaaabbbb
bab
bba
*/
/*
results
ab - 1
abb - 1
aaaabbbb - 1
bab - 0
bba - 0
*/