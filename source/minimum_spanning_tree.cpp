#include "../include/Graph.hpp"


Graph Graph::minimum_spanning_tree_by_kruskal()
{
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;  // Ordena as arestas pelo peso
    });

    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return agm;
}

Graph Graph::minimum_spanning_tree_by_prim()
{
    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return agm;
}
