//
// Created by sanch on 10/28/2019.
//

#ifndef INC_28_10_19_TOKEN_H
#define INC_28_10_19_TOKEN_H
#include "Biblios.h"
using namespace std;

enum class Kind : char {
    name, number, end, plus='+', minus='-', mul='*', div='/', print=';', assign='=', lp='(', rp=')'
};
struct Token {
    Kind kind;
    string string_value;
    double number_value;
};
#endif //INC_28_10_19_TOKEN_H
