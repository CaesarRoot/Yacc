//
// Created by 曾少勋 on 2019/12/15.
//

#ifndef YACC_PPT_H
#define YACC_PPT_H
#include "type.h"
#include <vector>
#include <map>
using namespace std;

class PPT{
private:
    // 第一个string为Vn，第二个string为Vt，Production为产生式
    map<string, map<string, Production>> table;
public:
    void add(Production p, string Vt);
    Production search(string Vn, string Vt);
    bool isVn(string s);
    friend void printPPT(const PPT &ppt);
};

#endif //YACC_PPT_H
