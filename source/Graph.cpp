#include "../include/Graph.hpp"

Graph::Graph(std::ifstream& instance)
{
    if(!instance.is_open())
    {
        std::cout << "Error: input file is not open\n";
        exit(1);
    }

    // inicializa variáveis
    std::string line, string_node_1, string_node_2, string_weight;
    size_t node_id_1, node_id_2, num_nodes(0);
    float weight;

    // lê o número de nós
    std::getline(instance, line);
    num_nodes = std::stoi(line);


    // lê os nós e arestas
    for(size_t i = 0; i < num_nodes; i++)
    {
        // lê uma linha do arquivo e separa os valores
        std::getline(instance, line);
        string_node_1 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_node_2 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_weight = line;

        // converte os valores para os tipos corretos
        node_id_1 = std::stoi(string_node_1);
        node_id_2 = std::stoi(string_node_2);
        weight = std::stof(string_weight);


        // adiciona os nós e arestas ao grafo //! verificar depois
        this->add_node(node_id_1);
        this->add_node(node_id_2);
        this->add_edge(node_id_1, node_id_2, weight);
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
    //Checa se não tem conexão entre os nós
    if(this->connected(node_id_1, node_id_2))
    {
        std::cout << "Error: nodes are already connected\n";
        return;
    }

    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
    {
        if(aux->id == node_id_1)
            node_1 = aux;
        if(aux->id == node_id_2)
            node_2 = aux;

        // Checa se os nós foram encontrados
        if(node_1 != nullptr && node_2 != nullptr)
            break;
    }

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found\n";
        return;
    }

    // Cria a aresta
    Edge *new_edge = new Edge;

    // Inicializa aresta para apontar para o nó 2
    new_edge->target_id = node_id_2;

    // Checa se a aresta é ponderada
    if(this->weighted_edges) new_edge->weight = weight;

    // Adiciona a aresta ao nó 1
    new_edge->next_edge = node_1->first_edge;
    node_1->first_edge = new_edge;
    node_1->number_of_edges++;

    // Checa se o grafo é direcionado, caso não seja, cria a aresta no nó 2
    if(!this->directed)
    { 
        // Recria a aresta
        new_edge = new Edge;

        // Inicializa aresta para apontar para o nó 1
        new_edge->target_id = node_id_1;

        // Checa se a aresta é ponderada
        if(this->weighted_edges) new_edge->weight = weight;

        // Adiciona a aresta ao nó 2
        new_edge->next_edge = node_2->first_edge;
        node_2->first_edge = new_edge;
        node_2->number_of_edges++;
    }


    // Incrementa o número de arestas
    this->number_of_edges++;

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
    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
    {
        if(aux->id == node_id_1)
            node_1 = aux;
        if(aux->id == node_id_2)
            node_2 = aux;

        // Checa se os nós foram encontrados
        if(node_1 != nullptr && node_2 != nullptr)
            break;
    }

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found\n";
        return;
    }

    // Checa se os nós estão conectados
    for(Edge *edge = node_1->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        if(edge->target_id == node_id_2)
            return 1;
    }
    
    return 0;
}
