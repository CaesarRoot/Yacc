//
// Created by 曾少勋 on 2019/12/15.
//

#ifndef YACC_TYPE_H
#define YACC_TYPE_H

#include <string>
#include <vector>

using namespace std;

typedef struct production {
    // 对应产生式的左部
    string Vn;
    // 产生式的右部
    vector<string> items;

    friend ostream &operator<<(ostream &os, production &p);

    // 自定义类型需要重载<使set中的值唯一
    int operator<(const production &p) const {
        if (Vn == p.Vn) {
            return items < p.items;
        }
        return Vn < p.Vn;
    }
} Production;

#endif //YACC_TYPE_H
