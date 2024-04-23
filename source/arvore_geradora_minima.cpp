#include "Graph.hpp"


Graph Graph::kruskal()
{
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;  // Ordena as arestas pelo peso
    });

    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return agm;
}

Graph Graph::prim()
{
    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return agm;
}
