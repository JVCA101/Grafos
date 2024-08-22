#include "../include/Graph.hpp"


void Graph::dfs_with_back_edges(size_t current_node, std::vector<size_t>& visited, DFS_Tree& dfs_tree, Back_edges& back_edges, size_t parent_node)
{
    visited[current_node] = 1;
    Node* node = this->get_node(current_node);

    for(Edge* edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        size_t next_node = edge->target_id;

        if(!visited[next_node])
        {
            // Adiciona a aresta na árvore de DFS
            dfs_tree[current_node].push_back(next_node);

            this->dfs_with_back_edges(next_node, visited, dfs_tree, back_edges, current_node);
        }
        else if(next_node != parent_node)
        {
            // Se o nó já foi visitado e não é o pai, é uma aresta de retorno
            back_edges.push_back({current_node, next_node});
        }
    }
}

DFS_Tree Graph::depth_first_tree(size_t start_node_id, Back_edges& back_edges)
{
    DFS_Tree dfs_tree;  // Armazena a árvore de DFS
    std::vector<size_t> visited(number_of_nodes, 0);  // Marca os nós visitados

    // Inicia a DFS a partir do nó fornecido
    this->dfs_with_back_edges(start_node_id, visited, dfs_tree, back_edges, start_node_id);

    return dfs_tree;
}
