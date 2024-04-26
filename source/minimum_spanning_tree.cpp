#include "../include/Graph.hpp"


Graph Graph::minimum_spanning_tree_by_kruskal()
{   

    // Ordena as arestas do grafo
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;  // Ordena as arestas pelo peso
    });

    // Cria um novo grafo para o AGM
    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);
    
    // Copia os nós do grafo original para o AGM sem as arestas
    for (auto node : this->get_nodes())
    {
        agm.add_node(node.id, node.weight);
    }

    // Contador de arestas adicionadas ao AGM
    int added_edges = 0;

    // Inicia os IDs das árvores do Nós como 1 de AGM como fora da árvore
    for (auto node : agm.get_nodes())
    {
        node.tree_id = 1;
    }
    
    // Percorre as arestas ordenadas e adiciona ao AGM caso não forme ciclo
    while (added_edges < agm.get_number_of_nodes() - 1)
    {
        // Pega a próxima aresta e a remove em seguida
        Edge edge = edges.front();
        edges.erase(edges.begin());
        
        // Se os nós da aresta não são da mesma subárvore, adiciona a aresta ao AGM
        if (!agm.get_node(edge.node_1).tree_id == 0 || !agm.get_node(edge.node_2).tree_id == 0)
        {
            agm.add_edge(edge.node_1, edge.node_2, edge.weight);
            added_edges++;
            
            // Se não estão em nenhum árvore, cria uma nova árvore
            if (!agm.get_node(edge.node_1).tree_id == 0 && !agm.get_node(edge.node_2).tree_id == 0)
            {
                agm.get_node(edge.node_1).tree_id = agm.get_node(edge.node_2).tree_id = agm.get_number_of_nodes();
            }
            // Se um dos nós está em uma árvore, adiciona o outro nó a mesma árvore
            else if (agm.get_node(edge.node_1).tree_id == 0)
            {
                agm.get_node(edge.node_1).tree_id = agm.get_node(edge.node_2).tree_id;
            }
            else if (agm.get_node(edge.node_2).tree_id == 0)
            {
                agm.get_node(edge.node_2).tree_id = agm.get_node(edge.node_1).tree_id;
            }
            // Se ambos estão em árvores diferentes, junta as árvores
            else
            {
                int tree_id_1 = agm.get_node(edge.node_1).tree_id;
                int tree_id_2 = agm.get_node(edge.node_2).tree_id;
                for (auto node : agm.get_nodes())
                {
                    if (node.tree_id == tree_id_2)
                    {
                        node.tree_id = tree_id_1;
                    }
                }
            }
    }

    // Retorna o AGM
    return agm;
}

Graph Graph::minimum_spanning_tree_by_prim()
{
    Graph agm(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return agm;
}
