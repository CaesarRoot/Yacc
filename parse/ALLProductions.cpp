//
// Created by 曾少勋 on 2019/12/15.
//
#include "ALLProductions.h"
#include <algorithm>

void ALLProductions::add(string production) {
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

    Production p{Vn, items};
    this->add(p);
    // 默认第一个产生式的左部为开始符号
    if (this->start.empty()) {
        this->start = Vn;
    }
}

void ALLProductions::add(Production production) {
    this->allProductions.insert(production);
    this->Vns.insert(production.Vn);
    if (this->sameVnProductions.find(production.Vn) == this->sameVnProductions.end()) {
        set<Production> sp;
        sp.emplace(production);
        this->sameVnProductions.emplace(production.Vn, sp);
    } else {
        this->sameVnProductions[production.Vn].emplace(production);
    }
}

bool ALLProductions::isVn(string item) {
    return !(this->Vns.find(item) == this->Vns.end());
}

set<string> ALLProductions::first(vector<string> items, set<vector<string>> &visited){
    if(visited.find(items) != visited.end()) {
        set<string> n;
        return n;
    }
    visited.insert(items);
    set<string> res;
    string item = items[0];
    // 如果是终结符（包括epsilon），那么直接加入结果集合，返回
    if (!this->isVn(item)) {
        res.insert(item);
        return res;
    } else {
        // 对于非终结符，则要递归求解
        // 因为提取最大公共左因子和消除左递归
        // 递归不会出现死循环和重复
        set<Production> products = this->sameVnProductions[item];
        // 对所有的产生式求first
        for (auto it = products.begin(); it != products.end(); it++) {
            vector<string> items = it->items;
            while (!items.empty()) {
                // 取出序列中的第一个求first
                set<string> tres = first(items, visited);
                // 如果结果中不包含epsilon边，则退出循环
                if (tres.find("@") == tres.end()) {
                    res.insert(tres.begin(), tres.end());
                    break;
                }
                    // 否则将除了epsilon之外的first集合元素加入结果集合中
                    // 并对下一个item求first
                else {
                    tres.erase("@");
                    res.insert(tres.begin(), tres.end());
                    items.erase(items.begin());
                }
                // 直到序列为空
            }
            // 如果items为空，则说明该产生式中每一个item的first都包含epsilon
            // 此时需要将epsilon加入结果集中
            if (items.empty()) {
                res.insert("@");
            }
        }
        return res;
    }
}

set<string> ALLProductions::first(vector<string> items) {
    set<vector<string>> visited;
    return this->first(items, visited);
}

set<string> ALLProductions::follow(string Vn) {
    set<string> visited;
    return this->follow(Vn, visited);
}

set<string> ALLProductions::follow(string Vn, set<string> &visited) {
    // 如果之前已经求解过了，则直接返回一个空集
    if (visited.find(Vn) != visited.end()) {
        set<string> n;
        return n;
    }
    // 开头就设置visited
    // 后续如果还是搜索到这个节点，直接返回
    // 因为只是求集合方程的最小解
    visited.insert(Vn);
    set<string> res;
    // 开始符号的follow一定包含$
    if (Vn == this->start) res.insert("$");
    // 遍历所有产生式的右部
    for (auto production: this->allProductions) {
        // 如果包含该非终结符
        vector<string>::iterator pos;
        if ((pos = find(production.items.begin(), production.items.end(), Vn)) != production.items.end()) {
            // 该非终结符还不在产生式的末尾
            if (pos != production.items.end() - 1) {
                vector<string> items;
                items.assign(pos + 1, production.items.end());
                set<string> firsts = this->first(items);
                // 如果没有epsilon
                if (firsts.find("@") == firsts.end()) {
                    res.insert(firsts.begin(), firsts.end());
                    // 只有这种情况才会跳过后面的流程
                    continue;
                } else {
                    // 如果有epsilon，那么将除了epsilon之外的边加入结果中
                    // 并且还要继续求解
                    firsts.erase("@");
                    res.insert(firsts.begin(), firsts.end());
                }
            }
            // 将该产生式的follow加入结果集中
            set<string> follows = this->follow(production.Vn, visited);
            res.insert(follows.begin(), follows.end());
        }
    }
    return res;
}
