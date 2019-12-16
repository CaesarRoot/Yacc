//
// Created by 曾少勋 on 2019/12/10.
//
#include "type.h"
#include <set>
#include <map>
#include <iostream>
#include "Utils.h"

using namespace std;

void dfsPrint(const Node *node, map<const Node *, int> &printMaps, set<const Node *> &printed);

void printTable(FA *fa) {
    map<const Node *, int> printMaps;
    set<const Node *> printed;
    dfsPrint(fa->start, printMaps, printed);
}

void dfsPrint(const Node *node, map<const Node *, int> &printMaps, set<const Node *> &printed) {
    if (printed.find(node) != printed.end()) return;
    int from;
    if (printMaps.find(node) == printMaps.end())
        printMaps.emplace(node, printMaps.size());
    from = printMaps[node];
    for (auto edge:node->next) {
        int to;
        if (printMaps.find(edge->node) == printMaps.end())
            printMaps.emplace(edge->node, printMaps.size());
        to = printMaps[edge->node];
        cout << "I" << from << " " << edge->value << " " << "I" << to << endl;
    }
    printed.emplace(node);
    if (!node->next.empty())
        for (auto edge:node->next)
            dfsPrint(edge->node, printMaps, printed);
}