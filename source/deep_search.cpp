#include "../include/Graph.hpp"

/**
 * @brief Percorre o grafo em profundidade e marca os nós visitados
 * 
 */
void Graph::basic_deep_search()
{
    // Marca todos os nós como não visitados
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        node->visited = false;

    // Chama a função recursiva
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
    {
        if(!node->visited)
            aux_basic_deep_search(node);
    }

}

/**
 * @brief Percorre o grafo em profundidade e marca os nós conectados
 * 
 */
void Graph::deep_search_connected_components(){
    // Marca todos os nós como não conectados
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        node->connection_mark = 0;

    int component = 0;

    // Chama a função recursiva
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
    {   
        // chama a função recursiva se o nó não foi visitado
        if(node->connection_mark == 0)
        {
            component++;
            aux_deep_search_connected_components(node, component);
        }
    }
}

/**
 * @brief Retorna o fecho transitivo direto de um nó
 * 
 * @param node_id id do nó que se deseja calcular o fecho transitivo
 * @return std::vector<Node> vetor de nós que compõem o fecho transitivo direto
 */
std::vector<Node> Graph::get_direct_transitive_closure(const size_t node_id){
    // Usando floyd para calcular
    float** matrix = this->shortest_path_floyd_matrix();
    std::vector<Node> transitive_closure;
    size_t n = this->number_of_nodes;

    // Adiciona os nós que o nó passado como parâmetro tem caminho
    for(size_t i = 0; i <= n; i++)
    {   
        // Checa se existe caminho entre os nós, percorrendo a linha
        if(matrix[node_id][i] != inf_f)
        {
            Node* node = this->get_node(i);
            if(node != nullptr)
                transitive_closure.push_back(*node);
        }
    }

    // deleta a matriz
    for(size_t i = 0; i < n; i++)
        delete[] matrix[i];

    return transitive_closure;
}

/**
 * @brief Retorna o fecho transitivo indireto de um nó
 * 
 * @param node_id id do nó que se deseja calcular o fecho transitivo
 * @return std::vector<Node> vetor de nós que compõem o fecho transitivo indireto
 */
std::vector<Node> Graph::get_inverse_transitive_closure(const size_t node_id){
    // Usando floyd para calcular
    float** matrix = this->shortest_path_floyd_matrix();
    std::vector<Node> transitive_closure;
    size_t n = this->number_of_nodes;

    // Adiciona os nós que tem caminho para o nó passado como parâmetro
    for(size_t i = 0; i < n; i++)
    {
        // Checa se existe caminho entre os nós, percorrendo a coluna
        if(matrix[i][node_id] != inf_f)
        {
            Node* node = this->get_node(i);
            if(node != nullptr)
                transitive_closure.push_back(*node);
        }
    }

    // delete matrix
    for(size_t i = 0; i < n; i++)
        delete[] matrix[i];

    return transitive_closure;
}

/**
 * @brief Retorna um subgrafo induzido pelos nós passados como parâmetro
 * 
 * @param nodes vetor de nós que compõem o subgrafo
 * @return Graph subgrafo induzido
 */
Graph Graph::subgraph_vertice_induced(const std::vector<Node> nodes)
{
    // Cria um novo grafo
    Graph subgraph(this->name + "_subgraph", this->directed, this->weighted_edges, this->weighted_nodes);

    // Adiciona os nós ao novo grafo
    for(auto node : nodes)
        subgraph.add_node(node.id, node.weight);

    // Adiciona as arestas entre os nós ao novo grafo
    for(Node *node = subgraph.first_node; node != nullptr; node = node->next_node)
    // Procura o nó no grafo original
        for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
        // Se o nó foi encontrado
            if(aux->id == node->id)
            // Adiciona as arestas
                for(Edge *edge = aux->first_edge; edge != nullptr; edge = edge->next_edge)
                    if(this->connected(aux->id, edge->target_id))
                        subgraph.add_edge(aux->id, edge->target_id, edge->weight);

    return subgraph;
}




//* Funções Auxiliares

void Graph::aux_remove_edge(Node *const node, const size_t node_position)
{
    Edge *prev_edge = nullptr;
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        if(edge->target_id == node_position)
        {
            // Caso seja a primeira aresta
            if(prev_edge == nullptr)
                node->first_edge = edge->next_edge;
            // Caso não seja a primeira aresta, atualiza o ponteiro da aresta anterior para o próximo	
            else
                prev_edge->next_edge = edge->next_edge;

            // Deleta a aresta
            delete edge;
            node->number_of_edges--;
            break;
        }

        prev_edge = edge;
    }
}

void Graph::aux_basic_deep_search(Node *const node)
{
    // Marca o nó como visitado
    node->visited = true;

    // Chama a função recursiva para os nós adjacentes
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        // Procura o nó
        Node* aux;
        for(aux = this->first_node; aux != nullptr; aux = aux->next_node)
        {
            if(aux->id == edge->target_id)
                break;
        }

        // Chama a função recursiva se o nó não foi visitado
        if(!aux->visited)
            aux_basic_deep_search(aux);
    }
}

void Graph::aux_deep_search_connected_components(Node *const node, const int connection_mark)
{
    // Marca o nó como visitado
    node->connection_mark = connection_mark;

    // Chama a função recursiva para os nós adjacentes
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        // Procura o nó
        Node* aux;
        for(aux = this->first_node; aux != nullptr; aux = aux->next_node)
        {
            // Se o nó foi encontrado
            if(aux->id == edge->target_id)
                break;
        }

        // Chama a função recursiva se o nó não foi visitado
        if(aux->connection_mark == 0)
            aux_deep_search_connected_components(aux, connection_mark);
    }
}
