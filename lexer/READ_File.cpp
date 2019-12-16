//
// Created by 曾少勋 on 2019/12/9.
//
#include <iostream>
#include <fstream>
#include <string>
#include "READ_file.h"
#include "type.h"
#include "RE_Suffix.h"

using namespace std;

void readFile(vector<RE> &regs, string fileName) {
    ifstream f;
    f.open(fileName, ios::in);
    if (!f) cout << "error" << endl;
    string type;
    string re;
    while (f >> type) {
        f >> re;
//        cout << type << endl;
//        cout << re << endl;
        re = parse_to_prefix_form(re);
        regs.push_back({type, re});
    }
    f.close();
}
