#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"

using Back_edges = std::vector<std::pair<size_t, size_t>>; // Arestas de retorno na busca em profundidade
using DFS_Tree = std::map<size_t, std::vector<size_t>>;    // Árvore de busca em profundidade

class Graph
{
public:

    struct Attributes {
        float ray;
        float diameter;
        std::vector<Node> center;
        std::vector<Node> periphery;
    };

    //* Funções Principais
    Graph(std::ifstream& instance, const bool directed = false, const bool weighted_edges = false, const bool weighted_nodes = false);
    Graph(const std::string name, const bool directed = false, const bool weighted_edges = false, const bool weighted_nodes = false);
    Graph() = default;
    Graph(const Graph& graph) = default;
    ~Graph();

    void remove_node(const size_t node_id);
    void remove_edge(const size_t node_id_1, const size_t node_id_2);
    void add_node(const size_t node_id, const float weight = 0.0);
    void add_edge(const size_t node_id_1, const size_t node_id_2, const float weight = 0.0);
    void print_graph(std::ofstream& output_file);
    void print_graph() const noexcept;
    int connected(const size_t node_id_1, const size_t node_id_2);


    // deep_search.cpp
    void basic_deep_search();
    void deep_search_connected_components();
    Graph subgraph_vertice_induced(std::vector<Node> nodes);
    std::vector<Node> get_direct_transitive_closure(const size_t node_id);
    std::vector<Node> get_inverse_transitive_closure(const size_t node_id);


    // minimum_spanning_tree.cpp
    Graph* minimum_spanning_tree_by_kruskal();
    Graph* minimum_spanning_tree_by_prim();


    // shortest_path.cpp
    std::vector<Node> shortest_path_dijkstra(const size_t origin, const size_t destination);
    float shortest_path_floyd(const size_t origin, const size_t destination);
    float** shortest_path_floyd_matrix();

    // depth_search_tree.cpp
    DFS_Tree depth_first_tree(const size_t start_node_id, Back_edges& back_edges);
    Graph::Attributes get_attributes();
    std::vector<Node> articulation_points();

    // minimum_gap_partition.cpp
    std::vector<Node> mggpp_partition_greedy(const unsigned int p);
    std::vector<Node> mggpp_greedy_randomized_adaptive(const unsigned int p);
    std::vector<Node> mggpp_greedy_randomized_adaptive_reactive(const unsigned int p);



    //* Getters
    std::vector<Node> get_nodes() const;
    std::vector<Node*> get_nodes_ptr() const;
    std::vector<Edge> get_edges() const;
    std::vector<Edge> get_edges(const size_t node_id) const;
    const std::string get_name() const noexcept;
    bool is_directed() const noexcept;
    bool has_weighted_edges() const noexcept;
    bool has_weighted_nodes() const noexcept;
    size_t get_number_of_nodes() const noexcept;
    Node* get_node(const size_t node_id) const;
    Edge* get_edge(const size_t node_id_1, const size_t node_id_2) const;


private:
    std::string name;
    size_t number_of_nodes;
    size_t number_of_edges;
    bool   directed;
    bool   weighted_edges;
    bool   weighted_nodes;
    Node  *first_node;
    Node  *last_node;


    //* Funções Auxiliares
    void search_nodes(Node *&node_1, const size_t node_id_1, Node *&node_2, const int node_id_2 = -1) const;
    void aux_remove_edge(Node *const node, const size_t node_position);
    void aux_basic_deep_search(Node *const node);
    void aux_deep_search_connected_components(Node *const node, const int connection_mark);
    size_t index_of_node(const size_t node_id);
    void dfs_articulation(std::vector<size_t>& vis, const size_t i, const size_t curr);
    void dfs_with_back_edges(const size_t current_node, std::vector<size_t>& visited, DFS_Tree& dfs_tree, Back_edges& back_edges, const size_t parent_node);
    void add_edge_to_mst(Graph& mst, Edge& edge, int& tree_id_1, int& tree_id_2, int& new_tree_id, size_t& added_edges);
    float weight_of_connection(Graph& graph, Node& node_1, Node& node_2);
    Node* inicialize_j(Graph& graph);

};

#endif  //GRAPH_HPP
