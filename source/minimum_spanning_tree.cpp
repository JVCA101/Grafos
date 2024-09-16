#include "../include/Graph.hpp"



/**
 * @brief Retorna a árvore geradora mínima do grafo utilizando o algoritmo de Kruskal
 * 
 * @return Graph árvore geradora mínima
 */
Graph* Graph::minimum_spanning_tree_by_kruskal()
{   

    // Pega todas as arestas do grafo e ordena pelo peso
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;
    });

    // Cria um novo grafo para a árvore geradora mínima
    Graph* mst = new Graph(this->name, this->directed, this->weighted_edges, this->weighted_nodes);
    
    // Copia todos os nós para a árvore geradora mínima sem arestas e "seta" o tree_id para 0
    auto nodes = this->get_nodes();
    for (auto node : nodes)
    {
        mst->add_node(node.id, node.weight);
        mst->get_node(node.id)->first_edge = nullptr;
        mst->get_node(node.id)->tree_id = 0;
    }

    // Variáveis auxiliares
    size_t added_edges = 0;

    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    int tree_id_1, tree_id_2, new_tree_id=1;
    size_t n_nodes = mst->get_number_of_nodes();
    Edge edge;

    
    // Enquanto não tiver adicionado todas as arestas e ainda houver arestas para adicionar
    while ((added_edges < n_nodes - 1) && (!edges.empty()))
    {
        // Pega a primeira aresta do vetor de arestas
        edge = edges[0];
        edges.erase(edges.begin());
        
        // Pega os nós da aresta
        node_1 = mst->get_node(edge.origin_id);
        node_2 = mst->get_node(edge.target_id);
        tree_id_1 = node_1->tree_id;
        tree_id_2 = node_2->tree_id;

        // Adiciona a aresta à árvore geradora mínima
        if((tree_id_1 != tree_id_2) || (tree_id_1 == 0 || tree_id_2 == 0))
            add_edge_to_mst(*mst, edge, tree_id_1, tree_id_2, new_tree_id, added_edges);
    }

    return mst;
}

/**
 * @brief Retorna a árvore geradora mínima do grafo utilizando o algoritmo de Prim
 * 
 * @return Graph árvore geradora mínima
 */
Graph* Graph::minimum_spanning_tree_by_prim()
{
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;
    });
    Edge edge_min = edges[0];

    Graph* mst = new Graph(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    // Copia todos os nós para a árvore geradora mínima sem arestas e "seta" o tree_id para 0
    for(auto node : this->get_nodes())
    {
        mst->add_node(node.id, node.weight);
        mst->get_node(node.id)->first_edge = nullptr;
        mst->get_node(node.id)->key = inf_f;
        mst->get_node(node.id)->tree_id = 0;
    }
    
    // Adiciona a primeira aresta à árvore geradora mínima
    mst->add_edge(edge_min.origin_id, edge_min.target_id, edge_min.weight);
    mst->get_node(edge_min.origin_id)->tree_id = 1;
    mst->get_node(edge_min.target_id)->tree_id = 1;
    
    float weight_origin, weight_target;


    for(auto node : mst->get_nodes_ptr())
    {
        // Ignora o nó que foi adicionado à árvore geradora mínima
        if(node->tree_id == 1)
            continue;

        // Calcula o peso da conexão entre o nó e os nós da árvore geradora mínima
        weight_origin = weight_of_connection(*this, *mst->get_node(edge_min.origin_id), *node);
        weight_target = weight_of_connection(*this, *mst->get_node(edge_min.target_id), *node);

        // Atualiza o valor de key e prox do nó
        if (weight_origin < weight_target){
            node->key = weight_origin;
            node->prox = mst->get_node(edge_min.origin_id);
        }
        else{
            node->key = weight_target;
            node->prox = mst->get_node(edge_min.target_id);
        }
    }

    size_t cont = 0;
    weight_origin = weight_target = 0;
    
    while(cont < mst->get_number_of_nodes() - 2)
    {
        Node *j = inicialize_j(*mst);

        if(j == nullptr)
            break;
        
        // Adiciona a aresta à árvore geradora mínima
        if(j->prox != nullptr)
        {
            mst->add_edge(j->prox->id, j->id, j->key);
            j->tree_id = 1;
            j->key = 0;
        }

        // Atualiza os valores de key e prox dos nós como já foi feito anteriormente
        for(auto node : mst->get_nodes_ptr())
        {
            if(node->tree_id)
                continue;

            weight_origin = weight_of_connection(*this, *mst->get_node(j->id), *node);
           
           if(weight_origin < node->key)
           {
               node->key = weight_origin;
               node->prox = mst->get_node(j->id);
           }

        }

        cont++;
    }

    for(auto& node : mst->get_nodes_ptr())
    {
        // Se o nó não foi adicionado à árvore geradora mínima, adiciona a aresta de menor peso
        if(node->tree_id == 0)
        {
            Node *prox = nullptr;
            float min = inf_f;

            for(auto& node_2 : mst->get_nodes_ptr())
            {
                if(node_2->tree_id == 1)
                {
                    float weight = weight_of_connection(*this, *node, *node_2);
                    if(weight < min)
                    {
                        min = weight;
                        prox = node_2;
                    }
                }
            }

            mst->add_edge(node->id, prox->id, min);
            node->tree_id = 1;
        }
    }

    
    return mst;
}



//* ------------------------------------------
//* ------------Auxiliar Functions------------
//* ------------------------------------------

void Graph::add_edge_to_mst(Graph& mst, Edge& edge, int& tree_id_1, int& tree_id_2, int& new_tree_id, size_t& added_edges)
{
    mst.add_edge(edge.origin_id, edge.target_id, edge.weight);
    added_edges++;

    // Atualiza o tree_id dos nós da árvore geradora mínima
    if(tree_id_1 == 0 && tree_id_2 == 0)
    {
        // Se os dois nós não estão na árvore geradora mínima, cria uma nova árvore
        mst.get_node(edge.origin_id)->tree_id = new_tree_id;
        mst.get_node(edge.target_id)->tree_id = new_tree_id;
        new_tree_id++;
    }
    else if(tree_id_1 == 0)
    // Se apenas o nó 1 não está na árvore geradora mínima, atualiza o tree_id do nó 1
        mst.get_node(edge.origin_id)->tree_id = tree_id_2;
    else if(tree_id_2 == 0)
    // Se apenas o nó 2 não está na árvore geradora mínima, atualiza o tree_id do nó 2
        mst.get_node(edge.target_id)->tree_id = tree_id_1;
    else if(tree_id_1 != tree_id_2)
    {
        // Se os dois nós estão em árvores diferentes, atualiza o tree_id das árvores para o mesmo valor
        auto nodes = mst.get_nodes_ptr();
        for (auto& node : nodes)
        {
            if (node->tree_id == tree_id_2)
                node->tree_id = tree_id_1;
        }
    }

}

float Graph::weight_of_connection(Graph& graph, Node& node_1, Node& node_2)
{
    if(graph.connected(node_1.id, node_2.id))
        return graph.get_edge(node_1.id, node_2.id)->weight;

    return inf_f;
}

Node* Graph::inicialize_j(Graph& graph)
{
    Node *j = nullptr;
    float min = inf_f;

    // Procura o nó com menor key que ainda não foi adicionado à árvore geradora mínima
    for(auto node : graph.get_nodes_ptr()){
        if(node->key < min && node->tree_id == 0)
        {
            min = node->key;
            j = graph.get_node(node->id);
        }
    }

    return j;
}
