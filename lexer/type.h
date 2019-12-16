//
// Created by 曾少勋 on 2019/12/8.
//

#ifndef LEXER_TYPE_H
#define LEXER_TYPE_H

#include <string>
#include <vector>
#include <set>

typedef struct node Node;

typedef struct edge {
    std::string value;
    Node *node;
} Edge;

typedef struct node {
    std::set<Edge *> next;
    std::set<std::string> type;    // 如果该节点是终态，那么type的值为该词法的名称
} Node;

typedef struct fa {
    Node *start;
    std::vector<Node *> end;
} FA;

typedef struct re {
    std::string name;
    std::string rules;
} RE;

#endif //LEXER_TYPE_H
