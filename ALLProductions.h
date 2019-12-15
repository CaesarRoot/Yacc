//
// Created by 曾少勋 on 2019/12/15.
//

#ifndef YACC_ALLPRODUCTIONS_H
#define YACC_ALLPRODUCTIONS_H

#include <set>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include "type.h"

using namespace std;

class ALLProductions {
private:
    string start;
    set<string> Vns;
    set<Production> allProductions;
    map<string, set<Production>> sameVnProductions;

    void add(Production production);

    set<string> follow(string Vn, set<string> &visited);

    set<string> first(vector<string> items, set<vector<string>> &visited);

public:
    set<string> first(vector<string> items);

    set<string> follow(string Vn);

    bool isVn(string item);

    void add(string production);

    friend void first_and_follow();
};

#endif //YACC_ALLPRODUCTIONS_H
