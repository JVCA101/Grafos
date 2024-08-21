#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "defines.hpp"


class Graph
{
public:
    /*Assinatura dos métodos básicos para o funcionamento da classe*/

    struct Attributes {
        float ray;
        float diameter;
        std::vector<Node> center;
        std::vector<Node> periphery;
    };

    //* Funções Principais
    Graph(std::ifstream& instance, bool directed = false, bool weighted_edges = false, bool weighted_nodes = false);
    Graph(std::string name, bool directed = false, bool weighted_edges = false, bool weighted_nodes = false);
    ~Graph();

    void remove_node(size_t node_id);
    void remove_edge(size_t node_id_1, size_t node_id_2);
    void add_node(size_t node_id, float weight = 0.0);
    void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0.0);
    void print_graph(std::ofstream& output_file);
    void print_graph() noexcept;
    int connected(size_t node_id_1, size_t node_id_2);


    // deep_search.cpp
    void basic_deep_search();
    void deep_search_connected_components();
    Graph subgraph_vertice_induced(std::vector<Node> nodes);
    std::vector<Node> get_direct_transitive_closure(size_t node_id);
    std::vector<Node> get_inverse_transitive_closure(size_t node_id);


    // minimum_spanning_tree.cpp
    Graph minimum_spanning_tree_by_kruskal();
    Graph minimum_spanning_tree_by_prim();


    // shortest_path.cpp
    std::vector<Node> shortest_path_dijkstra(size_t node_id_1, size_t node_id_2);
    float shortest_path_floyd(size_t node_id_1, size_t node_id_2);
    float** shortest_path_floyd_matrix();
    Graph::Attributes get_attributes();
    std::vector<Node> articulation_points();




    //* Getters
    std::vector<Node> get_nodes();
    std::vector<Node*> get_nodes_ptr();
    std::vector<Edge> get_edges();
    const std::string get_name() const noexcept;
    bool is_directed() const noexcept;
    bool has_weighted_edges() const noexcept;
    bool has_weighted_nodes() const noexcept;
    size_t get_number_of_nodes() const noexcept;
    Node* get_node(size_t node_id);
    Edge* get_edge(size_t node_id_1, size_t node_id_2);


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
    void search_nodes(Node *&node_1, const size_t node_id_1, Node *&node_2, const int node_id_2 = -1);
    void aux_remove_edge(Node *const node, const size_t node_position);
    void aux_basic_deep_search(Node *const node);
    void aux_deep_search_connected_components(Node *const node, const int connection_mark);
    size_t index_of_node(const size_t node_id);
    void dfs_articulation(std::vector<size_t>& vis, size_t i, size_t curr);

};

#endif  //GRAPH_HPP
