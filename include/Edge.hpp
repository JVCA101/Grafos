#ifndef GRAFO_BASICO_EDGE_H
#define GRAFO_BASICO_EDGE_H

#include "defines.hpp"

struct Edge
{
    Edge  *next_edge;
    float  weight;
    size_t origin_id;
    size_t target_id;
};

#endif /* GRAFO_BASICO_EDGE_H */
