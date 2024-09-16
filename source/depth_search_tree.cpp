#include "../include/Graph.hpp"

/**
 * @brief Função auxiliar para a busca em profundidade com arestas de retorno
 * 
 * @param current_node nó atual
 * @param visited vetor de visitados
 * @param dfs_tree árvore de busca em profundidade
 * @param back_edges arestas de retorno
 * @param parent_node nó pai
 */
void Graph::dfs_with_back_edges(const size_t current_node, std::vector<size_t>& visited, DFS_Tree& dfs_tree, Back_edges& back_edges, const size_t parent_node)
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

/**
 * @brief Retorna a árvore de busca em profundidade (DFS) a partir de um nó
 * 
 * @param start_node_id nó de início
 * @param back_edges arestas de retorno
 * @return DFS_Tree árvore de busca em profundidade
 */
DFS_Tree Graph::depth_first_tree(const size_t start_node_id, Back_edges& back_edges)
{
    DFS_Tree dfs_tree;  // Armazena a árvore de DFS
    std::vector<size_t> visited(number_of_nodes, 0);  // Marca os nós visitados

    // Inicia a DFS a partir do nó fornecido
    this->dfs_with_back_edges(start_node_id, visited, dfs_tree, back_edges, start_node_id);

    return dfs_tree;
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

    size_t n = this->get_number_of_nodes();
    float ray = inf_f;
    float diameter = 0;
    
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            // Procura o maior valor da matriz para encontrar o diâmetro
            if (matrix[i][j] > diameter && matrix[i][j] != inf_f)
                diameter = matrix[i][j];
            // Procura o menor valor da matriz para encontrar o raio
            if (matrix[i][j] < ray)
                ray = matrix[i][j];
        }
    }

    std::vector<Node> periphery;
    std::vector<Node> center;


    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++){
            // Adiciona os nós que estão na periferia e no centro
            if (matrix[i][j] == ray){
                Node* node = this->get_node(i+1);
                if(node != nullptr)
                    center.push_back(*node);
            } else if (matrix[i][j] == diameter){
                Node* node = this->get_node(i+1);
                if(node != nullptr)
                    periphery.push_back(*node);
            }
        }        
    }

    // delete matrix
    for(size_t i = 0; i < n; i++)
        delete[] matrix[i];

    return {ray, diameter, center, periphery};
}

/**
 * @brief Função auxiliar para o cálculo dos pontos de articulação
 * 
 * @param vis vetor de visitados
 * @param i id do nó
 * @param curr id do nó atual
 */
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

/**
 * @brief Retorna os pontos de articulação do grafo
 * 
 * @return std::vector<Node> pontos de articulação
 */
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
