#include "../include/Graph.hpp"
#include "../include/defines.hpp"


//* Funções principais

/**
 * @brief Construct a new Graph:: Graph object from a file
 * 
 * @param instance file with the graph
 */
Graph::Graph(std::ifstream& instance, const bool directed, const bool weighted_edges, const bool weighted_nodes)
{
    if(!instance.is_open())
    {
        std::cout << "Error: input file is not open\n";
        exit(1);
    }

    this->name = "graph";
    this->number_of_edges = 0;
    this->number_of_nodes = 0;
    this->directed = directed;
    this->weighted_edges = weighted_edges;
    this->weighted_nodes = weighted_nodes;
    this->first_node = nullptr;
    this->last_node = nullptr;

    // inicializa variáveis
    std::string line, string_node_1, string_node_2, string_weight;
    size_t node_id_1, node_id_2;
    float weight;

    // lê o número de nós
    std::getline(instance, line);
    const size_t num_nodes = std::stoi(line);
    std::cout << num_nodes << "\n";


    // lê os nós e arestas
    while(std::getline(instance, line))
    {
        node_id_1 = node_id_2 = 0;
        weight = 0.0;
        string_node_1 = string_node_2 = string_weight = "";

        // lê uma linha do arquivo e separa os valores
        string_node_1 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_node_2 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_weight = line;

        // converte os valores para os tipos corretos
        node_id_1 = std::stoi(string_node_1);
        node_id_2 = std::stoi(string_node_2);
        weight = std::stof(string_weight);

        std::cout << node_id_1 << " " << node_id_2 << " " << weight << std::endl;


        // adiciona os nós e arestas ao grafo
        this->add_node(node_id_1);
        this->add_node(node_id_2);
        this->add_edge(node_id_1, node_id_2, weight);
    }

}

/**
 * @brief Construct a new Graph:: Graph object from a file
 * It is used on the instances for the minimum gap graph partition problem
 */
Graph::Graph(std::ifstream& instance, Parameters parameters)
{
    if(!instance.is_open())
    {
        std::cout << "Error: input file is not open\n";
        exit(1);
    }

    std::string line, string_node_1, string_node_2, string_weight, string_aux;
    size_t node_id_1, node_id_2;

    // parameters
    this->directed = parameters.directed;
    this->weighted_edges = parameters.weighted_edges;
    this->weighted_nodes = parameters.weighted_nodes;

    std::getline(instance, line);

    // lê o nome do grafo, não conta o # do começo
    this->name = line.substr(2, line.size() - 1);
    this->name = this->name.substr(0, this->name.size() - 5);

    do
        std::getline(instance, line);
    while(line.substr(0, 5) != "param");

    // lê o número de partições, depois de aparecer "param p := "
    string_aux = line.substr(11, line.size() - 1);
    string_aux = string_aux.substr(0, string_aux.size() - 2);

    this->number_of_partitions = std::stoi(string_aux);


    // lê o número de nós
    std::getline(instance, line);
    std::getline(instance, line);
    string_aux = line.substr(2, line.size() - 10);

    const size_t num_nodes = std::stoi(string_aux);

    // lê os nós
    std::getline(instance, line);
    std::getline(instance, line);

    // vector com os ids dos nós separados por espaço
    std::vector<size_t> id_nodes;
    std::istringstream iss(line);
    string_aux = "";
    while(iss >> string_aux)
        id_nodes.push_back(std::stoi(string_aux));

    std::getline(instance, line);
    std::getline(instance, line);
    std::getline(instance, line);
    std::getline(instance, line);
    
    // le os pesos dos nos
    std::vector<float> weights(num_nodes, 0.0);
    int position_on_vector = -1;
    float weight_aux;
    while(line[0] != ';')
    {
        while (line[0] == ' ')
            line = line.substr(1, line.size() - 1);
        position_on_vector = std::stoi(line.substr(0, line.find(' ')));
        string_aux = line.substr(line.find(' '));
        string_aux = string_aux.substr(string_aux.find_first_not_of(' '));

        weight_aux = std::stof(string_aux);

        weights[position_on_vector-1] = weight_aux;

        std::getline(instance, line);
    }

    // add nodes
    for(size_t i = 0; i < num_nodes; i++)
        this->add_node(id_nodes[i], weights[i]);

    // lê as arestas
    std::getline(instance, line);
    std::getline(instance, line);
    std::getline(instance, line);
    std::getline(instance, line);
    
    while (line[0] != ';')
    {
        while (line.size() > 5)
        {
            while (line[0] == ' ')
                line = line.substr(1, line.size() - 1);
            string_aux = line.substr(1, line.find(')') - 1);
            node_id_1 = std::stoi(string_aux.substr(0, string_aux.find(',')));
            node_id_2 = std::stoi(string_aux.substr(string_aux.find(',') + 1));
            
            line = line.substr(line.find(')') + 1);

            this->add_edge(node_id_1, node_id_2);
        }
        
        std::getline(instance, line);
    }
    
}



/**
 * @brief Construct a new Graph:: Graph object from name, directed, weighted edges and weighted nodes
 * 
 * @param name 
 * @param directed 
 * @param weighted_edges 
 * @param weighted_nodes 
 */
Graph::Graph(const std::string name, const bool directed, const bool weighted_edges, const bool weighted_nodes)
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

/**
 * @brief Destroy the Graph:: Graph object
 * 
 */
Graph::~Graph()
{
    auto nodes_ptr = this->get_nodes_ptr();
    for(auto node : nodes_ptr)
    {
        auto edges_ptr = this->get_edges_ptr(node->id);
        for(auto edge : edges_ptr)
            if(edge != nullptr)
                delete edge;
    }

    
    // remove all nodes
    for(auto node : nodes_ptr)
        delete node;

    // Reseta as variáveis
    this->first_node = nullptr;
    this->last_node = nullptr;

    this->number_of_edges = 0;
    this->number_of_nodes = 0;

}

/**
 * @brief Remove um nó do grafo
 * 
 * @param node_position posição do nó
 */
void Graph::remove_node(const size_t node_position)
{
    // Procura o nó
    Node *prev_node = nullptr;
    Node *node = nullptr;
    for(node = this->first_node; node != nullptr; node = node->next_node)
    {
        if(node->id == node_position)
            break;
    }

    // Checa se o nó foi encontrado
    if(node == nullptr)
    {
        std::cout << "Error: node not found\n";
        return;
    }
    
    prev_node = node->previous_node;

    // Se for um grafo direcionado, remove as arestas que apontam para o nó
    if(this->directed)
    {
        for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
            aux_remove_edge(aux, node_position);
    }

    // Remove as arestas que saem do nó
    for(Edge *edge = node->first_edge; edge != nullptr; edge = node->first_edge)
    {
        node->first_edge = edge->next_edge;
        delete edge;

        // Decrementa o número de arestas
        this->number_of_edges--;
    }

    // Remove o nó

    // Caso só tenha um nó
    if(this->number_of_nodes == 1)
    {
        delete node;
        this->first_node = nullptr;
        this->last_node = nullptr;
    }
    else
    {
        // Caso o nó seja o primeiro
        if(prev_node == nullptr)
        {
            this->first_node = node->next_node;
            this->first_node->previous_node = nullptr;
        }
        else
        // Caso o nó seja o último
        if(node->next_node == nullptr)
        {
            prev_node->next_node = nullptr;
            this->last_node = prev_node;
        }
        else
        {
            prev_node->next_node = node->next_node;
            node->next_node->previous_node = prev_node;
        }

        delete node;
    }
   
    this->number_of_nodes--;
}

/**
 * @brief Remove uma aresta do grafo
 * 
 * @param node_position_1
 * @param node_position_2 
 */
void Graph::remove_edge(const size_t node_position_1, const size_t node_position_2)
{
    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    search_nodes(node_1, node_position_1, node_2, node_position_2);

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found\n";
        return;
    }

    // Remove a aresta do nó 1
    aux_remove_edge(node_1, node_position_2);

    // Checa se o grafo é direcionado, caso não seja, remove a aresta do nó 2
    if(!this->directed)
        aux_remove_edge(node_2, node_position_1);

    // Decrementa o número de arestas
    this->number_of_edges--;


}

/**
 * @brief Adiciona um nó ao grafo
 * 
 * @param node_id id do nó
 * @param weight peso do nó
 */
void Graph::add_node(const size_t node_id, const float weight)
{
    // Procura o nó
    size_t i = 0;
    for(Node *aux = this->first_node; i < number_of_nodes; aux = aux->next_node, i++)
        if(aux->id == node_id)
            return;
    
    //considerando weight = 0 e node id não contida no grafo
    Node *p   = new Node;
    p->first_edge = nullptr;

    // Checa se grafo é ponderado
    if(this->weighted_nodes){
        p->weight = weight;
    }

    p->id = node_id;

    if (this->number_of_nodes == 0)
    {
        p->next_node     = nullptr;
        p->previous_node = nullptr;
        first_node = p;
        last_node = p;
    }
    else
    {
        Node *aux = last_node;
        aux->next_node   = p;
        p->previous_node = aux;
        p->next_node = nullptr;
        last_node = p;
    }
    //considerar verificar se o nó foi adicionado com sucesso 
    number_of_nodes++;
}

/**
 * @brief Adiciona uma aresta ao grafo
 * 
 * @param node_id_1 id do nó 1
 * @param node_id_2 id do nó 2
 * @param weight peso da aresta
 */
void Graph::add_edge(const size_t node_id_1, const size_t node_id_2, const float weight)
{
    //Checa se não tem conexão entre os nós
    if(this->connected(node_id_1, node_id_2))
    {
        std::cout << "Error: nodes are already connected" << std::endl;
        return;
    }

    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    search_nodes(node_1, node_id_1, node_2, node_id_2);

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found" << std::endl;
        return;
    }

    // Cria a aresta
    Edge *new_edge = new Edge;
    new_edge->next_edge = node_1->first_edge;
    new_edge->origin_id = node_id_1;

    // Inicializa aresta para apontar para o nó 2
    new_edge->target_id = node_id_2;

    new_edge->weight = weight;

    // Adiciona a aresta ao nó 1
    auto aux = node_1->first_edge;
    node_1->first_edge = new_edge;
    new_edge->next_edge = aux;

    node_1->number_of_edges++;

    // Checa se o grafo é direcionado, caso não seja, cria a aresta no nó 2
    if(!this->directed)
    { 
        // Recria a aresta
        auto new_edge2 = new Edge;

        new_edge2->origin_id = node_id_2;

        // Inicializa aresta para apontar para o nó 1
        new_edge2->target_id = node_id_1;

        // Checa se a aresta é ponderada
        new_edge2->weight = weight;

        // Adiciona a aresta ao nó 2
        auto aux2 = node_2->first_edge;
        node_2->first_edge = new_edge2;
        new_edge2->next_edge = aux2;
        node_2->number_of_edges++;

        this->number_of_edges++;
    }


    // Incrementa o número de arestas
    this->number_of_edges++;

}

/**
 * @brief Escreve o grafo no terminal
 */
void Graph::print_graph() const noexcept
{
    // Verifica se o grafo é direcionado ou não e atribui o valor correto para o arquivo
    char edge_direction = '-';
    if(this->directed)
        edge_direction = '>';
    
    std::cout << "graph " << this->name << "{\n";

    // Escreve os nós e arestas no terminal
    for(auto node = this->first_node; node != this->last_node; node = node->next_node)
        for(auto edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
            std::cout << "  " << edge->origin_id << " -" << edge_direction << " " << edge->target_id << " " << edge->weight << "\n";
    std::cout << "\n}\n";
}

/**
 * @brief Escreve o grafo em um arquivo
 * 
 * @param output_file arquivo de saída
 */
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
        for(auto edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
            output_file << edge->origin_id << " -" << edge_direction << " " << edge->target_id << " " << edge->weight << ";\n";

    output_file << "\n}\n";

}

/**
 * @brief Verifica se dois nós estão conectados
 * 
 * @param node_id_1 primeiro id
 * @param node_id_2 segundo id
 * @return int 1 se estão conectados, 0 se não estão conectados, -1 se um dos nós não foi encontrado
 */
int Graph::connected(const size_t node_id_1, const size_t node_id_2)
{
    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    search_nodes(node_1, node_id_1, node_2, node_id_2);

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found\n";
        return -1;
    }

    if(node_1->first_edge == nullptr || node_2->first_edge == nullptr)
        return 0;

    // Checa se os nós estão conectados
    for(Edge *edge = node_1->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        if(edge->target_id == node_id_2)
            return 1;
    }

    return 0;
}




//* Funções auxiliares
/**
 * @brief Retorna o ponteiro para um nó
 * 
 * @param node_id id do nó
 * @return Node* ponteiro para o nó com o id passado
 */
Node* Graph::get_node(const size_t node_id) const
{
    for(Node* node = this->first_node; node != nullptr; node = node->next_node)
    {
        if(node->id == node_id)
            return node;
    }

    return nullptr;
}

/**
 * @brief Retorna a aresta entre dois nós
 * 
 * @param node_id_1 id do nó em que a aresta começa
 * @param node_id_2 id do nó em que a aresta termina
 * @return Edge* ponteiro para a aresta
 */
Edge* Graph::get_edge(const size_t node_id_1, const size_t node_id_2) const
{
    // Procura os nós
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    search_nodes(node_1, node_id_1, node_2, node_id_2);

    // Checa se os nós foram encontrados
    if(node_1 == nullptr || node_2 == nullptr)
    {
        std::cout << "Error: nodes not found\n";
        exit(1);
    }

    // Checa se os nós estão conectados
    for(Edge *edge = node_1->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        if(edge->target_id == node_id_2)
            return edge;
    }

    return nullptr;

}

/**
 * @brief Retorna todos os nós do grafo
 * 
 * @return std::vector<Node> vetor de nós
 */
std::vector<Node> Graph::get_nodes() const
{
    std::vector<Node> nodes;

    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        nodes.push_back(*node);

    return nodes;
}

/**
 * @brief Retorna todos os nós do grafo
 * 
 * @return std::vector<Node*> vetor de ponteiros para nós
 */
std::vector<Node*> Graph::get_nodes_ptr() const
{
    std::vector<Node*> nodes;

    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        nodes.push_back(node);

    return nodes;
}

/**
 * @brief Retorna todas as arestas do grafo
 * 
 * @return std::vector<Edge> vetor de arestas
 */
std::vector<Edge> Graph::get_edges() const
{
    std::vector<Edge> edges;

    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
            edges.push_back(*edge);

    return edges;
}

/**
 * @brief Retorna o nome do grafo
 * 
 * @return const std::string nome do grafo
 */
const std::string Graph::get_name() const noexcept
{
    return this->name;
}

/**
 * @brief Retorna se o grafo é direcionado ou não
 * 
 * @return true é direcionado
 * @return false não é direcionado
 */
bool Graph::is_directed() const noexcept
{
    return this->directed;
}

/**
 * @brief Retorna se o grafo possui arestas ponderadas
 * 
 * @return true tem arestas ponderadas
 * @return false não tem arestas ponderadas
 */
bool Graph::has_weighted_edges() const noexcept
{
    return this->weighted_edges;
}

/**
 * @brief Retorna se o grafo possui nós ponderados
 * 
 * @return true tem nós ponderados
 * @return false não tem nós ponderados
 */
bool Graph::has_weighted_nodes() const noexcept
{
    return this->weighted_nodes;
}

/**
 * @brief Retorna o número de nós do grafo
 * 
 * @return size_t número de nós
 */
size_t Graph::get_number_of_nodes() const noexcept
{
    return this->number_of_nodes;
}

void Graph::search_nodes(Node *&node_1, const size_t node_id_1, Node *&node_2, const int node_id_2) const
{
    for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
    {
        if(aux->id == node_id_1)
            node_1 = aux;
        if(node_id_2 != -1) // Se o segundo nó for -1, não faz sentido compará-lo com size_t
        {
            if(aux->id == (size_t)node_id_2)
                node_2 = aux;
        }
        // Checa se os nós foram encontrados
        if(node_1 != nullptr && (node_2 != nullptr || node_id_2 == -1))
            break;
    }
}

/**
 * @brief Retorna o índice de um nó
 * 
 * @param node_id id do nó
 * @return size_t índice do nó(-1 se não encontrado)
 */
size_t Graph::index_of_node(const size_t node_id)
{
    auto nodes = this->get_nodes();
    for(size_t i = 0; i < this->number_of_nodes; i++)
        if(nodes[i].id == node_id)
            return i;
    return -1;
}

/**
 * @brief 
 * 
 * @param node_id 
 * @return std::vector<Edge> 
 */
std::vector<Edge> Graph::get_edges(const size_t node_id) const
{
    std::vector<Edge> edges;

    Node *node = this->get_node(node_id);
    if(node == nullptr)
    {
        std::cout << "Error: node not found\n";
        exit(1);
    }

    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
        edges.push_back(*edge);

    return edges;
}

/**
 * @brief 
 * 
 * @param node_id 
 * @return std::vector<Edge*> 
 */
std::vector<Edge*> Graph::get_edges_ptr(const size_t node_id) const
{
    std::vector<Edge*> edges;
    Node *node = this->get_node(node_id);
    if(node == nullptr)
    {
        std::cout << "Error: node not found\n";
        exit(1);
    }

    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
        edges.push_back(edge);

    return edges;
}
