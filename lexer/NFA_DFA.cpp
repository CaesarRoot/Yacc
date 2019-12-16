//
// Created by 曾少勋 on 2019/12/9.
//
#include <iostream>
#include <set>
#include "type.h"
#include <queue>
#include <map>
#include "NFA_DFA.h"

using namespace std;

set<Node *> clousure(const set<Node *> &core, const FA *fa);

set<Node *> subset_construct(const set<Node *> &state, const FA *fa, string edge);

set<string> findAllEdge(set<Node *> state);


FA* NFA_to_DFA(FA *fa) {
    set<set<Node *>> allStates;
    // 用来形成边
    map<set<Node *>, Node *> maps;
    map<Node *, set<Node *>> reverseMap;
    // 状态和数字的映射
    map<set<Node *>, int> numsMap;
    queue<set<Node *>> to_search;

    // 第一个core为起始状态
    set<Node *> core;
    core.emplace(fa->start);
    set<Node *> state = clousure(core, fa);
    // 完全的初始状态进入状态集
    allStates.emplace(state);
    numsMap.emplace(state, allStates.size());
    to_search.push(state);
    Node* startPoint = new Node;
    maps.emplace(state, startPoint);
    reverseMap.emplace(startPoint, state);
    // 待搜索的状态队列不为空
    while (!to_search.empty()) {
        // 取出一个完全状态
        state = to_search.front();
        to_search.pop();
        Node *now = maps[state];
        // 找到所有出边
        set<string> allEdges = findAllEdge(state);
        for (auto edge : allEdges) {
            // 对每条边子集构造
            set<Node *> tempcore = subset_construct(state, fa, edge);
            if (!tempcore.empty()) {
                // 求闭包
                set<Node *> tempstate = clousure(tempcore, fa);
                // 如果这个状态之前没遇到过那么加入状态集合
                if (allStates.find(tempstate) == allStates.end()) {
                    allStates.emplace(tempstate);
                    to_search.push(tempstate);
                    // 加入一个新的节点
                    Node *newNode = new Node;
                    // 连接边
                    now->next.emplace(new Edge{edge, newNode});
                    maps.emplace(tempstate, newNode);
                    numsMap.emplace(tempstate, allStates.size());
                    reverseMap.emplace(newNode, tempstate);
                } else {
                    // 找到原有节点
                    // 连接上边
                    Node *oldNode = maps[tempstate];
                    now->next.emplace(new Edge{edge, oldNode});
                }
            }
        }
    }
    set<Node *> endPoints;
    for (auto tstate:allStates) {
        for (auto node:tstate) {
            if (!node->type.empty()) {
                Node *oneEnd = maps[tstate];
                oneEnd->type.insert(node->type.begin(), node->type.end());
                endPoints.emplace(oneEnd);
            }
        }
    }
    fa->start = startPoint;
    return fa;
}



/**
 * 找到所有的非空出边
 * @param state
 * @return
 */
set<string> findAllEdge(set<Node *> state) {
    set<string> ans;
    for (auto node:state) {
        for (auto e:node->next) {
            if (!e->value.empty())
                ans.emplace(e->value);
        }
    }
    return ans;
}


/**
 * @param state
 * 接受的state为完整的状态集合
 * 并不是core
 * 传入此方法之前要先对core求闭包
 * @param fa
 * @param edge
 * edge为当前待求解的出边
 * 返回核core
 */
set<Node *> subset_construct(const set<Node *> &state, const FA *fa, string edge) {
    set<Node *> core;
    for (auto node:state) {
        for (auto e:node->next)
            if (e->value == edge)
                core.emplace(e->node);
    }
    return core;
}

set<Node *> clousure(const set<Node *> &core, const FA *fa) {
    set<Node *> state;
    queue<Node *> q;
    for (auto n:core) {
        q.push(n);
    }
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        // 如果还没有求解过
        if (state.find(node) == state.end()) {
            // 加入已经求解过的状态集合
            state.emplace(node);
            // 循环所有出边
            for (auto e:node->next) {
                // 如果是空的边那么将下一个点加入等待搜索队列
                if (e->value.empty() && (state.find(e->node) == state.end())) {
                    // 留待下一次搜索的时候再加入状态集合
                    q.push(e->node);
                }
            }
        }
    }
    return state;
}

void predict(string word, Node *startPoint) {
    string empty = "";
    Node *node = startPoint;
    for (auto it = word.begin(); it != word.end(); it++) {
        bool flag = true;
        for (auto edge:node->next) {
            if (edge->value == (empty + (*it))) {
                node = edge->node;
                flag = false;
                break;
            }
        }
        if (flag) {
            cout << "invalid word " << word << " !!!!!!!!" << endl;
            return;
        }
    }
    if (node->type.size() > 1) {
        for (auto t:node->type) {
            if (t != "ID") {
                cout << t << endl;
                break;
            }
        }
    } else {
        for (auto t:node->type) {
            cout << t << endl;
        }
    }
}

