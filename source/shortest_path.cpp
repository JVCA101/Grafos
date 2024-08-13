#include "../include/Graph.hpp"

std::vector<Node> Graph::shortest_path_dijkstra(size_t node_id_1, size_t node_id_2)
{
    // std::vector<Node> S_barra;
    // std::vector<Node> S;
    // std::vector<double> pi;

    // for(size_t i = 0; i < number_of_nodes; i++)
    // {
    //     if(i == node_id_1)
    //     {
    //         S.push_back(*get_node(i));
    //         pi.push_back(0);
    //     }
    //     else
    //     {
    //         S_barra.push_back(*get_node(i));
    //         pi.push_back(std::numeric_limits<double>::infinity());
    //     }
    // }


    // while(S_barra.size() > 0)
    // {
    //     //TODO
    //     // Node *u = nullptr;
    //     // double min = std::numeric_limits<double>::infinity();
    //     // for(auto node : S_barra)
    //     // {
    //     //     if(pi[node.get_id()] < min)
    //     //     {
    //     //         min = pi[node.get_id()];
    //     //         u = get_node(node.get_id());
    //     //     }
    //     // }

    //     // S.push_back(*u);
    //     // S_barra.erase(std::remove(S_barra.begin(), S_barra.end(), *u), S_barra.end());

    //     // for(auto edge : u->get_edges())
    //     // {
    //     //     Node *v = get_node(edge.get_node_id());
    //     //     if(pi[v->get_id()] > pi[u->get_id()] + edge.get_weight())
    //     //     {
    //     //         pi[v->get_id()] = pi[u->get_id()] + edge.get_weight();
    //     //     }
    //     // }
    // }


}

float Graph::shortest_path_floyd(size_t node_id_1, size_t node_id_2)
{
    //TODO
    auto nodes = this->get_nodes();
    size_t n = this->number_of_nodes;
    float A[n][n];
    Edge* edge;

    size_t node_1 = index_of_node(node_id_1);
    size_t node_2 = index_of_node(node_id_2);

    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
        {
            edge = get_edge(nodes[i].id, nodes[j].id);
            if(edge == nullptr)
                A[i][j] = std::numeric_limits<float>::infinity();
            else
                A[i][j] = edge->weight * !(i == j);
        }


    for(size_t k = 0; k < n; k++)
        for(size_t i = 0; i < n; i++)
            for(size_t j = 0; j < n; j++)
                A[i][j] = std::min(A[i][j], A[i][k] + A[k][j]);

    return A[node_1][node_2];
}

size_t Graph::index_of_node(const size_t node_id)
{
    auto nodes = this->get_nodes();
    for(size_t i = 0; i < this->number_of_nodes; i++)
        if(nodes[i].id == node_id)
            return i;
    return -1;
}
