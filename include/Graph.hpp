#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"

class Graph
{
public:
    /*Assinatura dos métodos básicos para o funcionamento da classe*/

    Graph(std::ifstream& instance);
    Graph(std::string name, bool directed = false, bool weighted_edges = false, bool weighted_nodes = false);
    ~Graph();

    void remove_node(size_t node_id);
    void remove_edge(size_t node_id_1, size_t node_id_2);
    void add_node(size_t node_id, float weight = 0.0);
    void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0.0);
    void print_graph(std::ofstream& output_file);
    void print_graph();

    int connected(size_t node_id_1, size_t node_id_2);

private:
    std::string name;
    size_t number_of_nodes;
    size_t number_of_edges;
    bool   directed;
    bool   weighted_edges;
    bool   weighted_nodes;
    Node  *first_node;
    Node  *last_node;
};

#endif  //GRAPH_HPP
