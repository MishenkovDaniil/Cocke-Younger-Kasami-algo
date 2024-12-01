#include "cyk.hpp"
#include <cstring>

bool CYK::predict(const char *string) {
    size_t n = strlen(string);
    bool ***d = new bool**[256];
    for (size_t A = 0; A < 256; ++A) {
        d[A] = new bool*[n];
        for (size_t i = 0; i < n; ++i) {
            d[A][i] = new bool[n]; 
        }
    }

    for (size_t A = 0; A < 256; ++A) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                d[A][i][j] = false;
            }
        }
    }

    // for (auto A: grammar_->neTerminals_){
        
    //     //d[A][i][i] = true if A->w[i]
    // }

    for (auto rule: grammar_->rules_){
        if (rule.right_.size() == 1 && !(rule.right_[0] == Epsilon)) {
            char symbol = rule.right_[0].symbol_;
            for (size_t i = 0; i < n; ++i) {
                if (string[i] == symbol){
                    d[rule.left_.symbol_][i][i] = true;
                    printf ("d[%c][%d][%d] = true\n", rule.left_.symbol_, i, i);
                }
            }
        }   
    }

    for (size_t m = 1; m < n; ++m) {
        for (auto net:grammar_->neTerminals_) {
            char A = net.symbol_;
            for (size_t i = 0; i + m < n; ++i) {
                size_t j = m + i;
                for (auto rule: grammar_->rules_) {
                    if (rule.left_ == net && rule.right_.size() == 2){ // A->BC
                        for (size_t k = i; k < j; ++k) {
                            d[A][i][j] = d[rule.right_[0].symbol_][i][k] & d[rule.right_[1].symbol_][k + 1][j];
                            if (d[A][i][j]){
                                break;
                            }
                        }
                        if (d[A][i][j]){
                            break;
                        }
                    }
                }
            }
        }
    }
    return d[grammar_->start_.symbol_][0][n - 1];
}
