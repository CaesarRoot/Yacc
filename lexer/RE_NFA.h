//
// Created by 曾少勋 on 2019/12/9.
//

#ifndef LEXER_RE_NFA_H
#define LEXER_RE_NFA_H

#include <vector>
#include "type.h"

FA* REs_to_NFA(std::vector<RE> Regs);

#endif //LEXER_RE_NFA_H
