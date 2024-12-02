#include "cyk.hpp"
#include <cstring>

bool CYK::predict(const char *string) {
    size_t n = strlen(string);
    bool ***d = new bool**[neTerminalsSize_];
    for (size_t A = 0; A < neTerminalsSize_; ++A) {
        d[A] = new bool*[n];
        for (size_t i = 0; i < n; ++i) {
            d[A][i] = new bool[n]; 
        }
    }

    for (size_t A = 0; A < neTerminalsSize_; ++A) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                d[A][i][j] = false;
            }
        }
    }

    for (size_t neTerminalIdx = 0; neTerminalIdx < neTerminalsSize_; ++neTerminalIdx) {
        NeTerminal A = map_[neTerminalIdx];
        for (auto rule: grammar_->rules__[A]) {
            if (rule.right_.size() == 1) {
                char symbol = rule.right_[0].symbol_;
                for (size_t strIdx = 0; strIdx < n; ++strIdx) {
                    if (string[strIdx] == symbol){
                        d[neTerminalIdx][strIdx][strIdx] = true;
                    }
                }
            }
        }
    }

    // for (auto rule: grammar_->rules_){
    //     if (rule.right_.size() == 1 && !(rule.right_[0] == Epsilon)) {
    //         char symbol = rule.right_[0].symbol_;
    //         for (size_t i = 0; i < n; ++i) {
    //             if (string[i] == symbol){
    //                 d[rule.left_.symbol_][i][i] = true;
    //                 // printf ("d[%c][%d][%d] = true\n", rule.left_.symbol_, i, i);
    //             }
    //         }
    //     }   
    // }

    for (size_t len = 1; len < n; ++len) {
        for (size_t start = 0; start + len < n; ++start) {
            size_t end = len + start;
            for (size_t neTerminalIdx = 0; neTerminalIdx < neTerminalsSize_; ++neTerminalIdx) {
                NeTerminal A = map_[neTerminalIdx];
                for (auto& rule: grammar_->rules__[A]) {
                    if (rule.right_.size() == 2) {
                        int neTerminal1 = reverseMap_[rule.right_[0]];
                        int neTerminal2 = reverseMap_[rule.right_[1]];
                        for (size_t k = start; k < end; ++k) {
                            d[neTerminalIdx][start][end] = d[neTerminal1][start][k] & d[neTerminal2][k + 1][end];
                            if (d[neTerminalIdx][start][end]) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }


    // for (size_t m = 1; m < n; ++m) {
    //     for (auto net:grammar_->neTerminals_) {
    //         char A = net.symbol_;
    //         for (size_t i = 0; i + m < n; ++i) {
    //             size_t j = m + i;
    //             for (auto rule: grammar_->rules_) {
    //                 if (rule.left_ == net && rule.right_.size() == 2){ // A->BC
    //                     for (size_t k = i; k < j; ++k) {
    //                         d[A][i][j] = d[rule.right_[0].symbol_][i][k] & d[rule.right_[1].symbol_][k + 1][j];
    //                         if (d[A][i][j]){
    //                             break;
    //                         }
    //                     }
    //                     if (d[A][i][j]){
    //                         break;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

/*

map A: vector<vector<GrammarSymbol>> in grammar
map 1:A 2:B in cyk

for (size_t m = 0; m > n; ++m) {
    for (i, j j - i = m. j < n) {
        for k 
            A = map[k]
            for vectors in map[A]
                if (len(vectors) = 2)
                    d[A][i][j] == ....
                if d[[[]]]
                break
    }
}

*/


    int start = reverseMap_[grammar_->start_];
    return d[start][0][n - 1];
}
