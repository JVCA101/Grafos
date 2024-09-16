#include "../include/Graph.hpp"

/**
 * @brief Calcula o menor caminho entre dois nós utilizando o algoritmo de Dijkstra
 * 
 * @param origin nó de origem
 * @param destination nó de destino
 * @return std::vector<Node> vetor de nós que compõem o menor caminho
 */
std::vector<Node> Graph::shortest_path_dijkstra(const size_t origin, const size_t destination)
{
    std::vector<Node> S_barra;
    std::vector<Node> S;

    std::vector<float> pi(this->number_of_nodes);

    Node* node_1 = this->get_node(origin);

    std::vector<size_t> predecessors(this->number_of_nodes, max_size_t);

    std::vector<Node> nodes = this->get_nodes();
    // Inicializa o vetor de predecessores
    for(size_t i = 0; i < nodes.size(); i++)
    {
        // Se o nó for o nó de origem, o predecessor é ele mesmo
        if(nodes[i].id == origin)
        {
            S.push_back(nodes[i]);
            pi[i] = 0;
        }
        else
        {
            // Se o nó não for o nó de origem, checa se ele é adjacente ao nó de origem
            // Se for, o peso da conexão é o peso da aresta, caso contrário, o peso é infinito
            pi[i] = weight_of_connection(*this, *node_1, nodes[i]);
            S_barra.push_back(nodes[i]);
        }
    }

    
    while(!S_barra.empty())
    {
        float min = inf_f;
        size_t id_min = max_size_t;
        size_t index = 0;

        // Encontra o nó com menor valor de pi
        for(size_t i = 0; i < S_barra.size(); i++)
        {
            const Node& node = S_barra[i];
            if(pi[i] < min)
            {
                min = pi[i];
                id_min = node.id;
                index = i;
            }
        }


        // Se não houver nó com valor de pi menor que infinito, encerra o loop
        if(id_min == max_size_t)
            break;
        

        // Remove o nó com menor valor de pi de S_barra
        S_barra.erase(S_barra.begin() + index);
        
        Node *u = get_node(id_min);
        std::vector<Edge*> u_edges;
        S.push_back(*u);

        // Se o nó com menor valor de pi for o nó de destino, encerra o loop
        if(id_min == destination)
            break;
        
        // Adiciona as arestas do nó com menor valor de pi ao vetor de arestas
        for(Edge* edge = u->first_edge; edge != nullptr; edge = edge->next_edge)
            u_edges.push_back(edge);



        // Atualiza o valor de pi dos nós adjacentes ao nó com menor valor de pi
        for(auto& edge : u_edges)
        {
            const size_t v_id = edge->target_id;
            const float weight = edge->weight;

            if(pi[v_id] > pi[u->id] + weight)
            {
                pi[v_id] = pi[u->id] + weight;
                predecessors[v_id] = u->id;
            }
        }


    }
    
    return S;
    
}

/**
 * @brief Calcula o custo do menor caminho entre dois nós utilizando o algoritmo de Floyd
 * 
 * @param origin nó de origem
 * @param destination nó de destino
 * @return float custo do menor caminho
 */
float Graph::shortest_path_floyd(const size_t origin, const size_t destination)
{
    // Cria a matriz de adjacência
    auto nodes = this->get_nodes();
    size_t n = this->number_of_nodes;
    float** A = new float*[n];
    for(size_t i = 0; i < n; ++i)
        A[i] = new float[n];
    Edge* edge;

    size_t node_1 = index_of_node(origin);
    size_t node_2 = index_of_node(destination);

    // Inicializa a matriz de adjacência
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
        {
            // Se não houver aresta entre os nós, o valor é infinito
            edge = get_edge(nodes[i].id, nodes[j].id);
            if(edge == nullptr)
                A[i][j] = inf_f;
            else
                A[i][j] = edge->weight * !(i == j);
        }

    
    for(size_t k = 0; k < n; k++)
        for(size_t i = 0; i < n; i++)
            for(size_t j = 0; j < n; j++)
            // Atualiza o valor da matriz de adjacência, se o caminho entre os nós for menor que o valor atual
                A[i][j] = std::min(A[i][j], A[i][k] + A[k][j]);

    float result = A[node_1][node_2];

    // Deleta a matriz de adjacência
    for(size_t i = 0; i < n; i++)
        delete[] A[i];

    return result;
}

/**
 * @brief Calcula a matriz de adjacência com os menores caminhos entre todos os nós utilizando o algoritmo de Floyd
 * 
 * @return float** matriz de adjacência com os menores caminhos
 */
float** Graph::shortest_path_floyd_matrix()
{
    auto nodes = this->get_nodes();
    size_t n = this->number_of_nodes;
    float** A = new float*[n];
    for(size_t i = 0; i < n; ++i)
        A[i] = new float[n];
    Edge* edge;

    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
        {
            edge = get_edge(nodes[i].id, nodes[j].id);
            if(edge == nullptr)
                A[i][j] = inf_f;
            else
                A[i][j] = edge->weight * !(i == j);
        }

    for(size_t k = 0; k < n; k++)
        for(size_t i = 0; i < n; i++)
            for(size_t j = 0; j < n; j++)
                A[i][j] = std::min(A[i][j], A[i][k] + A[k][j]);

    return A;
}
