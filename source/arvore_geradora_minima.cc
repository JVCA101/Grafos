#include "arvore_geradora_minima.h"


Graph kruskal(Graph& graph)
{
    std::vector<Edge> edges = graph.get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;  // Ordena as arestas pelo peso
    });

    // Graph agm(graph.get_name(), graph.is_directed(), graph.has_weighted_edges(), graph.has_weighted_nodes());

    // std::vector<Node> nodes = graph.get_nodes();
    // std::vector<size_t> components;

    // for (size_t i = 0; i < nodes.size(); i++)
    // {
    //     components.push_back(i);
    //     agm.add_node(i, nodes[i].get_weight());
    // }

    // for (size_t i = 0; i < edges.size(); i++)
    // {
    //     size_t component_1 = components[edges[i].get_source_id()];
    //     size_t component_2 = components[edges[i].get_target_id()];

    //     if (component_1 != component_2)
    //     {
    //         agm.add_edge(edges[i].get_source_id(), edges[i].get_target_id(), edges[i].get_weight());

    //         for (size_t j = 0; j < components.size(); j++)
    //         {
    //             if (components[j] == component_2)
    //             {
    //                 components[j] = component_1;
    //             }
    //         }
    //     }
    // }

    // return agm;
}

Graph prim(Graph& graph)
{

}
