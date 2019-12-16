//
// Created by 曾少勋 on 2019/12/9.
//
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include "type.h"
#include "DFA_DFAo.h"

using namespace std;

bool alreadyExistEqual(const set<set<Edge *>> &edgeGroups,
                       const set<Edge *> &edges,
                       map<Node *, int> &keyMaps);

set<pair<string, int>> changeEdges(const set<Edge *> &edges, map<Node *, int> &keyMaps);

void addAllNode(Node *node, set<Node *> &nodes);

/**
 * @param maps 所有的DFA状态节点
 */
FA *DFA_to_DFAo(FA *fa) {
    set<Node*>allNodes;
    addAllNode(fa->start, allNodes);

    // 存储优化后的状态
    vector<set<Node *>> optimized;
    // 状态分组的唯一编号
    // 用来唯一标识一个分组
    int groupKey = 0;
    // 状态到组号的映射
    // 加速判断到达的点是否属于一个状态
    map<Node *, int> keyMaps;
    // 初始步骤
    // 先划分出终态 和 非终态的点的集合
    set<Node *> notEnd;
    for (auto node :allNodes) {
        // 非终态先都加入一个集合中
        if (node->type.empty()) {
            notEnd.insert(node);
            keyMaps.emplace(node, groupKey++);
        } else {
            // 如果是终态
            // 那么看看是否已经有相同的终态的集合
            // 如果有则直接加入
            // 如果还没有，则新增加一个集合
            bool found = false;
            // 这里要加上引用，否则无法更改原vector中的set
            for (auto &se:optimized) {
                for (auto e:se) {
                    // 一样的终态，则加入这个终态集合
                    // 这里的==是set的比较操作
                    // 因为目前采取的策略是关键字和标识符一起识别
                    // 因此同一个点的终态是个set
                    // 后续判断的时候再取舍
                    // 这里终态相等即终态的set相等
                    if (e->type == node->type) {
                        se.insert(node);
                        found = true;
                        keyMaps.emplace(node, keyMaps[e]);
                        break;
                    }
                }
                if (found) break;
            }
            // 此终态无已经加入结果集合的等价状态
            // 新建终态加入
            if (!found) {
                set<Node *> end = {node};
                optimized.push_back(end);
                keyMaps.emplace(node, groupKey++);
            }
        }
    }
    // 将非终止态放入结果集
    if (!notEnd.empty())
        optimized.push_back(notEnd);

    // 下面正式开始化简

    // 用一个变量标识是否有新的状态加入
    bool changed = true;
    while (changed) {
        changed = false;
        // 临时存储优化后的状态
        // 这里不直接添加到结果集合是因为要把老的状态删除
        // 因此这里采取的方法是每轮计算都重新赋值整个optimized
        vector<set<Node *>> new_optimized;

        // 遍历当前的所有状态集合
        for (auto state:optimized) {
            int newStateCount = 0;
            // 临时存储一个状态集合的优化后的状态集合
            // 这里不直接添加到new_optimized是为了区别有新状态集合和无新状态集合
            // 因为如果当前状态集合不可再分（即无新状态集合加入），则只要将原先的state加入即可，无需创建新的状态集合以及对应的groupKey
            // 否则如果直接加入新的state，则会产生新的groupKey，而状态集合还是旧的状态集合，这样就会产生矛盾
            map<set<pair<string, int>>, set<Node *>> newStatesMap;
            for (auto node:state) {
                // 将边和到达的点转化为边和到达的组号
                set<pair<string, int>> edge_group = changeEdges(node->next, keyMaps);
                // 如果转化后的出边的集合已经存在
                // 那么加入这个出边集合对应的状态集合
                if (newStatesMap.find(edge_group) != newStatesMap.end()) {
                    newStatesMap[edge_group].emplace(node);
                }
                    // 当前不存在此出边集合
                    // 新建一个状态集合
                else {
                    set<Node *> newState = {node};
                    newStatesMap.emplace(edge_group, newState);
                    newStateCount++;
                }
            }
            // newStateCount为1表示并无新增状态集合
            // >1表示有新建状态集合
            if (newStateCount > 1) {
                changed = true;
                // 遍历每一个状态集合
                for (auto nsm:newStatesMap) {
                    // 取出状态集合中的每一个状态
                    set<Node *> newState;
                    for (auto node:nsm.second) {
                        newState.emplace(node);
                        // 如果是该状态集合的第一个加入的状态
                        if (newState.size() == 1) {
                            // 那么加此状态对应到一个新的groupKey
                            // 这个groupKey也即是这个状态集合的groupKey
                            // 之后加入这个集合的状态的groupKey都等于这个值
                            keyMaps[node] = groupKey++;
                        } else
                            // 减1是因为前面是一个++操作
                            // 为了保证同一个集合中的groupKey都相等必须要-1
                            keyMaps[node] = groupKey - 1;
                    }
                    // 新建状态集合加入临时结果集合
                    new_optimized.push_back(newState);
                }
            } else {
                // 没有新增状态集合的话就加入老的状态集合
                new_optimized.push_back(state);
            }
        }
        // 更新一下当前求出的优化后的状态
        optimized = new_optimized;
    }
    // 下面将优化后的状态集合的图关系转为节点
    // 需要将一个状态集合对应到一个节点

    // 存储组号和新建节点的映射
    // 这里的组号即映射之后状态集合的groupKey，等于该状态集合中每一个状态keyMaps中对应的的groupKey
    map<int, Node *> reversedMap;
    // 最终的DFAo
    FA *finalFA = new FA;
    // 为所有状态新建一个代表节点
    for (auto state:optimized) {
        // 新建一个代表节点
        Node *representative = new Node;
        reversedMap.emplace(keyMaps[*state.begin()], representative);
    }
    for (auto state:optimized) {
        // 取出当前状态的代表节点
        Node *representative = reversedMap[keyMaps[*state.begin()]];
        // 找出状态集的所有出边
        // 用该状态集的任意一个状态皆可，因为是等价的
        set<pair<string, int>> oldStartEdges = changeEdges((*(state.begin()))->next, keyMaps);
        // 遍历原来的起点的出边
        for (auto edge:oldStartEdges) {
            Node *node = reversedMap[edge.second];
            representative->next.emplace(new Edge{edge.first, node});
        }
        // 加入所有结束状态（如果有的话）
        set<string> endTypes;
        for (auto node:state) {
            if (!node->type.empty()) {
                endTypes.insert(node->type.begin(), node->type.end());
            }
        }
        representative->type = endTypes;
        // 如果该状态包含原来的起点节点
        // 那么将最后的状态机的起始状态设置为这个新建的代表节点
        if (state.find(fa->start) != state.end())
            finalFA->start = representative;
    }

//    cout << optimized.size() << endl;
    return finalFA;
}

/**
 * 将边和到达的点转化为边和到达的组号
 * @param edges
 * @param keyMaps
 * @return
 */
set<pair<string, int>> changeEdges(const set<Edge *> &edges, map<Node *, int> &keyMaps) {
    set<pair<string, int>> edge_group;
    for (auto edge:edges) {
        edge_group.emplace(edge->value, keyMaps[edge->node]);
    }
    return edge_group;
}

void addAllNode(Node *node, set<Node *> &nodes) {
    nodes.emplace(node);
    for (auto e:node->next) {
        if (nodes.find(e->node) == nodes.end()) {
            nodes.emplace(e->node);
            addAllNode(e->node, nodes);
        }
    }
}

