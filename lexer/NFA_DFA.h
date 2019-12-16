//
// Created by 曾少勋 on 2019/12/9.
//

#ifndef LEXER_NFA_DFA_H
#define LEXER_NFA_DFA_H

#include "type.h"
#include <map>
#include <set>

using namespace std;

FA* NFA_to_DFA(FA *fa);
void predict(string word, Node *startPoint);

#endif //LEXER_NFA_DFA_H
