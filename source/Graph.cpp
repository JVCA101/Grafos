#include "../include/Graph.hpp"

Graph::Graph(std::ifstream& instance)
{
    if(!instance.is_open())
    {
        std::cout << "Error: input file is not open\n";
        exit(1);
    }
    
}

Graph::Graph(std::string name, bool directed, bool weighted_edges, bool weighted_nodes)
{
    this->name = name;

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
    // Verifica se o arquivo de saída está aberto
    if(!output_file.is_open())
    {
        std::cout << "Error: output file is not open\n";
        exit(1);
    }

    // escrita em arquivo .dot
    output_file << "graph " << this->name << " {\n";

    // Verifica se o grafo é direcionado ou não e atribui o valor correto para o arquivo
    char edge_direction = '-';
    if(this->directed)
    {
        edge_direction = '>';
    }

    // Escreve os nós e arestas no arquivo
    for(auto node = this->first_node; node != this->last_node; node = node->next_node)
        for(size_t i = 0; i < node->number_of_edges; i++)
            output_file << node->id << " -" << edge_direction << " " << node->first_edge[i].target_id << ";\n";

    output_file << "\n}\n";

}

int Graph::connected(size_t node_id_1, size_t node_id_2)
{
    return 0;
}
