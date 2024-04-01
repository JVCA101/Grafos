#include "../include/Graph.hpp"

Graph::Graph(std::ifstream& instance)
{
}

Graph::Graph(bool directed, bool weighted_edges, bool weighted_nodes)
{
    this->number_of_edges = 0;
    this->number_of_nodes = 0;

    this->directed = directed;
    this->weighted_edges = weighted_edges;
    this->weighted_nodes = weighted_nodes;

    this->first_node = nullptr;
    this->last_node = nullptr;
}

Graph::~Graph()
{
}

void Graph::remove_node(size_t node_position)
{
}

void Graph::remove_edge(size_t node_position_1, size_t node_position_2)
{
}

void Graph::add_node(size_t node_id, float weight)
{
}

void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight)
{
}

void Graph::print_graph()
{
}

void Graph::print_graph(std::ofstream& output_file)
{
}

int Graph::connected(size_t node_id_1, size_t node_id_2)
{
    return 0;
}
