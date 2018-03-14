//
//  NFA.cpp
//  JavaCompiler
//
//  Created by Ahmed on 3/13/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#include "NFA.hpp"

NFA:: NFA(Graph &g, int start_node, int end_node) {
    this->g = g;
    this->start_node = start_node;
    this->end_node = end_node;
}

Graph* NFA:: get_graph() {
    return &g;
}
int NFA:: get_start_node() {
    return start_node;
}

int NFA:: get_end_node() {
    return end_node;
}

int NFA:: get_priority() {
    return priority;
}
void NFA:: set_graph(const Graph &g) {
    this->g = g;
}
void NFA:: set_start_node(int node) {
    start_node = node;
}
void NFA:: set_end_node(int node) {
    end_node = node;
}
void NFA:: set_priority(int priority) {
    this->priority = priority;
}