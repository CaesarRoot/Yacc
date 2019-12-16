//
// Created by 曾少勋 on 2019/12/8.
//
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "type.h"
#include "RE_NFA.h"

using namespace std;

void print(Node *node);

FA *REs_to_NFA(vector<RE> Regs) {
    string empty = "";
    stack<FA *> NFAs;
    for (auto re:Regs) {
        for (auto it = re.rules.begin(); it != re.rules.end(); it++) {
            char ch = *it;
            if (ch == '\\') continue;

            // switch里面不能new
            if (ch == '|' && *(it - 1) != '\\') {
                FA *former2 = NFAs.top();
                NFAs.pop();
                FA *former1 = NFAs.top();
                NFAs.pop();
                Node *newHead = new Node;
                // 新建一个初始节点
                newHead->next.emplace(new Edge{"", former1->start});
                newHead->next.emplace(new Edge{"", former2->start});
                // 新建一个终止节点
                Node *newEnd = new Node;
                former1->end[0]->next.emplace(new Edge{"", newEnd});
                former2->end[0]->next.emplace(new Edge{"", newEnd});
                FA *newFA = new FA;
                newFA->start = newHead;
                newFA->end.push_back(newEnd);
                delete former1;
                delete former2;
                NFAs.push(newFA);
                continue;
            } else if (ch == '~' && *(it - 1) != '\\') {
                FA *former2 = NFAs.top();
                NFAs.pop();
                FA *former1 = NFAs.top();
                NFAs.pop();
                // 将former1的终态和former2的起始状态连接
                // 这里结束状态都只有一个
                former1->end[0]->next.emplace(new Edge{"", former2->start});
                FA *newFA = new FA;
                newFA->start = former1->start;
                newFA->end.assign(former2->end.begin(), former2->end.end());
                NFAs.push(newFA);
                // 这里delete只是释放掉指针
                // 节点仍然在堆上
                delete former1;
                delete former2;
                continue;
            } else if (ch == '*' && *(it - 1) != '\\') {
                FA *former1 = NFAs.top();
                NFAs.pop();
                FA *newFA = new FA;
                Node *newHead = new Node;
                Node *newEnd = new Node;
                newHead->next.emplace(new Edge{"", former1->start});
                newHead->next.emplace(new Edge{"", newEnd});
                former1->end[0]->next.emplace(new Edge{"", newEnd});
                former1->end[0]->next.emplace(new Edge{"", former1->start});
                newFA->start = newHead;
                newFA->end.push_back(newEnd);
                NFAs.push(newFA);
                delete former1;
                continue;
            } else {
                // 新建开始和结束节点
                Node *start = new Node;
                Node *end = new Node;
                // 连接开始和结束
                start->next.emplace(new Edge{empty + ch, end});
                FA *fa = new FA;
                // 记录开始节点
                fa->start = start;
                // 记录结束节点
                fa->end.push_back(end);
                // 压入临时栈
                NFAs.push(fa);
            }
        }
        FA *top = NFAs.top();
        NFAs.pop();
        // 加上一个小自动机的终态名字
        top->end[0]->type.emplace(re.name);
        NFAs.push(top);
    }
    FA *final = NFAs.top();
    NFAs.pop();
    while (!NFAs.empty()) {
        FA *former1 = NFAs.top();
        NFAs.pop();
        Node *newHead = new Node;
        newHead->next.emplace(new Edge{"", former1->start});
        newHead->next.emplace(new Edge{"", final->start});
        final->start = newHead;
        final->end.insert(final->end.begin(), former1->end.begin(), former1->end.end());
        delete former1;
    }
    //print(final->start);

    return final;
}

/*
 * 对a*递归打印会出现死循环
 */
void print(Node *node) {
    cout << *(node->type.begin());
    for (auto a:node->next) {
        if (a->value == "") cout << "@\t";
        else cout << a->value << "\t";
    }
    cout << endl;
    for (auto a:node->next) {
        print(a->node);
    }
}
