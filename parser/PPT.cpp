//
// Created by 曾少勋 on 2019/12/15.
//
#include "PPT.h"
using namespace std;

void PPT::add(Production p, string Vt){
    if(Vt == "@") return;
    string Vn = p.Vn;
    if(this->table.find(Vn) == this->table.end()){
        map<string, Production> t;
        t.emplace(Vt, p);
        this->table.emplace(Vn, t);
    } else {
        this->table[Vn].emplace(Vt, p);
    }
}

Production PPT::search(string Vn, string Vt){
    if(this->table.find(Vn) != this->table.end()){
        if(this->table[Vn].find(Vt) != this->table[Vn].end()){
            return this->table[Vn][Vt];
        }
    }
    Production p;
    return p;
}

bool PPT::isVn(string s){
    return !(this->table.find(s) == this->table.end());
}

