//
// Created by 曾少勋 on 2019/12/8.
//
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "RE_Suffix.h"

using namespace std;

string parse_to_prefix_form(string);

void add_dot(string &);

string parse_to_prefix_form(string reg) {
    if (reg.size() == 1) return reg;
    if (reg == "\\|\\|") return "\\|\\|~";
    if (reg == "\\*") return "\\*";
    map<string, int> prio = {
            pair<string, int>("@", 0),
            pair<string, int>("|", 1),
            pair<string, int>("~", 2),
            pair<string, int>("(", 3),
            pair<string, int>("*", 4),
            pair<string, int>(")", 5)};
    add_dot(reg);
    string empty = "";
    // 优先级最低的结尾符号
    reg += "@";
    stack<string> ch;
    stack<string> op;
    string prefix = "";
    for (auto it = reg.cbegin(); it != reg.cend(); it++) {
        // 字符直接进栈
        if ((*it) != '(' && (*it) != ')' && (*it) != '|' && (*it) != '*' && (*it) != DOT && (*it) != '@')
            ch.push(empty + (*it));
        else {
            // 遇到右括号就弹栈
            // 直到遇到左括号
            if ((*it) == ')') {
                while (op.top() != "(") {
                    string formerop = op.top();
                    // 双目运算符
                    if (formerop != "*") {
                        string former2 = ch.top();
                        ch.pop();
                        string former1 = ch.top();
                        ch.pop();
                        op.pop();
                        string temp = former1 + former2 + formerop;
                        ch.push(temp);
                    } else {
                        // 单目运算符
                        string former1 = ch.top();
                        ch.pop();
                        op.pop();
                        string temp = former1 + formerop;
                        ch.push(temp);
                    }
                }
                op.pop();
                continue;
            } else if (!op.empty()) {
                // 如果是其他符号
                string formerop = op.top();
                // 左括号只有右括号能弹栈
                if (formerop == "(") {
                    op.push(empty + (*it));
                    continue;
                }
                // 比较优先级
                // 如果优先级比当前优先级高或者相等
                // 那么出栈
                while (!op.empty() && op.top() != "(" && (prio[formerop] >= prio[empty + (*it)])) {
                    formerop = op.top();
                    // 双目运算符
                    if (formerop != "*") {
                        string former2 = ch.top();
                        ch.pop();
                        string former1 = ch.top();
                        ch.pop();
                        op.pop();
                        string temp = former1 + former2 + formerop;
                        ch.push(temp);
                    } else {
                        // 单目运算符
                        string former1 = ch.top();
                        ch.pop();
                        op.pop();
                        string temp = former1 + formerop;
                        ch.push(temp);
                    }
                }
                op.push(empty + (*it));
            } else op.push(empty + (*it));
        }
    }
    return ch.top();
}

void add_dot(string &reg) {
    if (reg.size() == 1) return;
    for (auto it = reg.begin(); it != reg.end(); it++) {
        if ((*it) == '(' || (*it) == '|') {
            continue;
        } else if ((it + 1) != reg.end()) {
            if ((*(it + 1)) != ')' && (*(it + 1)) != '|' && (*(it + 1)) != '*') {
                // insert在指定位置之前插入
                // 返回插入的位置
                it = reg.insert(it + 1, DOT);
            }
        } else {
            continue;
        }
    }
}
