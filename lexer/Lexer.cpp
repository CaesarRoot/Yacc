//
// Created by 曾少勋 on 2019/12/8.
//
#include <iostream>
#include "RE_Suffix.h"
#include "RE_NFA.h"
#include "type.h"
#include "NFA_DFA.h"
#include "READ_file.h"
#include <map>
#include <set>
#include "DFA_DFAo.h"
#include "Utils.h"
#include <fstream>

using namespace std;

void monitor(FA *fa, string fileName);

void printResult(string line, int start, int end, Node *node);

int main() {
    vector<RE> regs;
    readFile(regs, "resources/RE.l");

    FA *fa = REs_to_NFA(regs);
    fa = NFA_to_DFA(fa);
//    printTable(fa);
    cout << " ----------------- " << endl;
    FA *finalFA = DFA_to_DFAo(fa);
//    printTable(finalFA);

    monitor(finalFA, "resources/hello.c");

//    predict("aaaaaaaa", finalFA->start);
//    predict("10.0", finalFA->start);
//    predict("int", finalFA->start);
//    predict("3", finalFA->start);
//    predict("while", finalFA->start);
//    predict("auto", finalFA->start);
//    predict("=", finalFA->start);
//    predict("f", finalFA->start);
//    predict("+=", finalFA->start);

    return 0;
}

void monitor(FA *fa, string fileName) {
    ifstream file;
    file.open(fileName, ios::in);
    string line;
    Node *node = fa->start;
    const string empty = "";
    while (getline(file, line)) {
        // 插入空格，统一处理
        for (auto it = line.begin(); it != line.end(); it++) {
            if (*(it) == '(' || *(it) == ')' || *(it) == '{' || *(it) == '}' || *(it) == ';' || *(it) == '[' || *(it) == ']') {
                it = line.insert(it, ' ');
                it++;
                it = line.insert(it+1, ' ');
            }
        }
        int start = 0;
        int end = 0;
        // 略过开头的空格和会车
        while (line[end] == ' ')
            end++;
        if (line[end] == '\n') continue;
        start = end;
        node = fa->start;
        while (end < line.size()) {
            char ch = (char) line[end];
            // 找到空格或者会车，那么跳转到输出结果环节（仍然有可能出错）
            if (ch == ' ' || ch == '\n') {
                printResult(line, start, end, node);
                if (ch == '\n') break;
                // 略过剩下的空格
                while (line[end] == ' ')
                    end++;
                if (line[end] == '\n') break;
                start = end;
                node = fa->start;
                continue;
            }
            bool found = false;
            for (auto edge:node->next) {
                if (edge->value == (empty + ch)) {
                    node = edge->node;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "word: " << line.substr(start, end) << " is not a valid word !!!" << endl;
                // 如果发现词法分析出错，那么就跳过这个词，直到下一个空格或者会车
                while (line[end] != ' ' && line[end] != '\n') end++;
                start = ++end;
            } else end++;
        }
        if(start != end)
            printResult(line, start, end, node);
    }
}

void printResult(string line, int start, int end, Node *node) {
    string lexeme = line.substr(start, end - start);
    if (node->type.empty())
        cout << "word: " << lexeme << " is not a valid word !!!" << endl;
    if (node->type.size() == 1) cout << lexeme << " " << *(node->type.begin()) << endl;
    else {
        // 目前只有标识符和关键字会重复
        for (auto t:node->type) {
            if (t != "ID") {
                cout << lexeme << " " << t << endl;
                return;
            }
        }
    }
}
