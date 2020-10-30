#include <iostream>
#include "type.h"
#include "PPT.h"
#include <fstream>
#include <vector>
#include <queue>
#include "ALLProductions.h"

using namespace std;

void initPPT(PPT &ppt);

void printPPT(const PPT &ppt);

void first_and_follow();

queue<string> read_token_sequence(string filename);

int main() {
    first_and_follow();
    PPT ppt;
    initPPT(ppt);
    cout << "-------------------------" << endl;
//    printPPT(ppt);
    cout << "-------------------------" << endl;
    queue<string> readers = read_token_sequence("resources/token_sequence.txt");
    readers.push("$");

    stack<string> stack;
    stack.push("functions");

    vector<Production> productions;

    string Vt = readers.front();
    readers.pop();

    bool error = false;
    while (!stack.empty()) {
        // 栈顶为非终结符
        if (ppt.isVn(stack.top())) {
            string Vn = stack.top();
            stack.pop();
            Production p = ppt.search(Vn, Vt);
            // 如果没有找到对应的产生式则报错
            if (p.Vn.empty()) {
                error = true;
                cout << "error" << endl;
                break;
            } else {
                productions.push_back(p);
                // 将产生式反着压入栈中
                for (auto it = p.items.rbegin(); it != p.items.rend(); it++) {
                    stack.push(*it);
                }
            }
        }
            // 栈顶为终结符
        else {
            // 如果非终结符一样，说明匹配
            if (stack.top() == Vt) {
                stack.pop();
                Vt = readers.front();
                readers.pop();
            }
                // 如果栈顶是epsilon，那么直接弹栈
            else if (stack.top() == "@") {
                stack.pop();
            }
                // 否则说明出现错误
            else {
                error = true;
                cout << "error!!!" << endl;
                break;
            }
        }
    }
    if (!error) {
        for (auto it = productions.begin(); it != productions.end(); it++) {
            cout << it->Vn << " -> ";
            for (auto item:it->items) {
                cout << item << " ";
            }
            cout << endl;
        }
    }
    return 0;
}

queue<string> read_token_sequence(string filename) {
    ifstream f;
    f.open(filename, ios::in);
    string line;
    queue<string> res;
    while (1) {
        getline(f, line);
        if (line.length() == 0) break;
        res.push(line);
    }
    return res;
}

void first_and_follow() {
    ALLProductions allProductions;
    ofstream of;
    of.open("resources/productions.txt", ios::out);
    ifstream f;
    f.open("resources/plainProductions.txt", ios::in);
    string line;
    while (getline(f, line)) {
        allProductions.add(line);
    }
    // 对所有产生式求first
    for (auto p:allProductions.allProductions) {
//        cout << p;
        of << p;
        set<string> firsts = allProductions.first(p.items);
        // 如果first中包含epsilon边，那么还要求follow
        if (firsts.find("@") != firsts.end()) {
            set<string> follows = allProductions.follow(p.Vn);
            firsts.insert(follows.begin(), follows.end());
        }
        for (auto it = firsts.begin(); it != firsts.end(); it++) {
            if (it != firsts.begin()) {
//                cout << " ";
                of << " ";
            }
//            cout << *it;
            of << *it;
        }
//        cout << endl;
        of << endl;
    }
};

void initPPT(PPT &ppt) {
    ifstream f;
    f.open("resources/productions.txt", ios::in);
    string production;
    while (getline(f, production)) {
        if (production == "end") break;
        if (production.length() == 0) continue;
        int pos = production.find('-');
        string Vn = production.substr(0, pos - 1);
        string rights = production.substr(pos + 3);
        int former = -1;
        vector<string> items;
        while ((pos = rights.find(' ', former + 1)) != string::npos) {
            items.push_back(rights.substr(former + 1, pos - former - 1));
            former = pos;
        }
        items.push_back(rights.substr(former + 1));

        Production p;
        p.Vn = Vn;
        p.items = items;
        string Vts;
        while (getline(f, Vts)) {
            if (Vts.length() == 0) continue;
            else break;
        }
        former = pos = -1;
        while ((pos = Vts.find(' ', former + 1)) != string::npos) {
            ppt.add(p, Vts.substr(former + 1, pos - former - 1));
            former = pos;
        }
        ppt.add(p, Vts.substr(former + 1));
    }
}

void printPPT(const PPT &ppt) {
    for (auto pp: ppt.table) {
        for (auto p:pp.second) {
            cout << p.second;
            cout << p.first << endl;
        }
    }
}