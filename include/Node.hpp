#ifndef GRAFO_BASICO_NODE_H
#define GRAFO_BASICO_NODE_H

#include "Edge.hpp"
#include "defines.hpp"

struct Node
{
    size_t number_of_edges;
    size_t id;
    float  weight;
    bool   visited;
    int   connection_mark;
    Edge  *first_edge;
    Node  *next_node;
    Node  *previous_node;
};

#endif  //GRAFO_BASICO_NODE_H
