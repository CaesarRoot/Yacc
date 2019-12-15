//
// Created by 曾少勋 on 2019/12/15.
//
#include "type.h"
#include <iostream>

using namespace std;

ostream &operator<<(ostream &os, production &p) {
    os << p.Vn << " -> ";
    for (auto it = p.items.begin(); it != p.items.end(); it++){
        if(it!=p.items.begin()) os << " ";
        os << *it;
    }
    os << endl;
    return os;
}