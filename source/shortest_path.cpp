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
    for(size_t i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].id == origin)
        {
            S.push_back(nodes[i]);
            pi[i] = 0;
        }
        else
        {
            pi[i] = weight_of_connection(*this, *node_1, nodes[i]);
            S_barra.push_back(nodes[i]);
        }
    }


    while(!S_barra.empty())
    {
        float min = inf_f;
        size_t id_min = max_size_t;
        size_t index = 0;

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


        if(id_min == max_size_t)
            break;
        

        S_barra.erase(S_barra.begin() + index);

        Node *u = get_node(id_min);
        std::vector<Edge*> u_edges;
        S.push_back(*u);

        if(id_min == destination)
            break;
        
        for(Edge* edge = u->first_edge; edge != nullptr; edge = edge->next_edge)
            u_edges.push_back(edge);



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
    auto nodes = this->get_nodes();
    size_t n = this->number_of_nodes;
    float** A = new float*[n];
    for(size_t i = 0; i < n; ++i)
        A[i] = new float[n];
    Edge* edge;

    size_t node_1 = index_of_node(origin);
    size_t node_2 = index_of_node(destination);

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

    return A[node_1][node_2];
}

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

/**
 * @brief Retorna os atributos do grafo
 * 
 * @param graph grafo
 * @return GraphAttributes atributos do grafo
 */
Graph::Attributes Graph::get_attributes()
{
    float** matrix = this->shortest_path_floyd_matrix();
    // number of nodes
    size_t n = this->get_number_of_nodes();
    // minimal value of matrix
    float ray = inf_f;
    // maximal value of matrix
    float diameter = 0;
    
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            if (matrix[i][j] > diameter && matrix[i][j] != inf_f)
                diameter = matrix[i][j];
            if (matrix[i][j] < ray)
                ray = matrix[i][j];
        }
    }

    std::vector<Node> periphery;
    std::vector<Node> center;


    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++){
            if (matrix[i][j] == ray){
                Node* node = this->get_node(i);
                if(node != nullptr)
                    center.push_back(*node);
            } else if (matrix[i][j] == diameter){
                Node* node = this->get_node(i);
                if(node != nullptr)
                    periphery.push_back(*node);
            }
        }
        
    }

    return {ray, diameter, center, periphery};
}

size_t Graph::index_of_node(const size_t node_id)
{
    auto nodes = this->get_nodes();
    for(size_t i = 0; i < this->number_of_nodes; i++)
        if(nodes[i].id == node_id)
            return i;
    return -1;
}


void Graph::dfs_articulation(std::vector<size_t>& vis, const size_t i, const size_t curr)
{
    vis[curr] = 1;
    Node* const current_node = this->get_node(curr);
    if(current_node == nullptr)
        return;
    
    for(Edge* e = current_node->first_edge; e != nullptr; e = e->next_edge)
    {
        size_t x = e->target_id;
        if (x != i && vis[x] == 0)
        {
            this->dfs_articulation(vis, i, x);
        }
    }
}

std::vector<Node> Graph::articulation_points() {
    std::vector<Node> articulation_points;

    // Iterando sobre todos os vértices do grafo
    for (size_t i = 0; i < number_of_nodes; i++) {
        size_t components = 0;
        std::vector<size_t> vis(number_of_nodes, 0);

        // Iterando sobre o grafo após remover o vértice i
        for (size_t j = 0; j < number_of_nodes; j++) {
            if (j != i && vis[j] == 0) {
                components++;
                this->dfs_articulation(vis, i, j);
            }
        }

        // Se o número de componentes for maior que 1 após remover
        // o vértice i, então i é um ponto de articulação.
        if (components > 1) {
            Node* node = get_node(i);
            if(node != nullptr)
                articulation_points.push_back(*node);
        }
    }

    return articulation_points;
}