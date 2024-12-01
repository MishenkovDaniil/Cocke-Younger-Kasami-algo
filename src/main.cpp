
#include <iostream>
#include <sstream>
#include "grammar/grammar.hpp"
#include "cyk.hpp"
int main () {
    // NeTerminal a('a');
    // NeTerminal f('a');

    // std::cout << (a == f);


    
    // std::istringstream s1("Hello, world.");
    // for (int i = 1; i < 33; ++i) {
    //     char c1 = s1.get(); // reads 'H'
    //     std::cout << "after reading [" << int(c1) << "], gcount() == " <<  s1.gcount() << '\n';
    // }
    // std::istringstream s1("Hello, world.\n");
    // char c1 = 0;
    // for (int i = 1; i < 33; ++i) {
    //     s1 >> c1;
    //     std::cout << "after reading [" << c1 << "], gcount() == " <<  s1.gcount() << '\n';
    // }

    Grammar grammar;
    grammar.CreateFromStdin();
    grammar.Print();

    CYK cyk;
    cyk.fit(grammar);
    bool res = cyk.predict("ab");
    std::cout << "res for ab = " << res << "\n";
    res = cyk.predict("abb");
    std::cout << "res for abb = " << res << "\n";

    res = cyk.predict("aab");
    std::cout << "res for aab = " << res << "\n";
    res = cyk.predict("aaaabbbb");
    std::cout << "res for aaaabbbb = " << res << "\n";
    res = cyk.predict("bab");
    std::cout << "res for bab = " << res << "\n";
}