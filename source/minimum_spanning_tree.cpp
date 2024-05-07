#include "../include/Graph.hpp"

void add_edge_to_mst(Graph& mst, Edge& edge, size_t& tree_id_1, size_t& tree_id_2, size_t& new_tree_id, size_t& added_edges);

Graph Graph::minimum_spanning_tree_by_kruskal()
{   

    // Get all edges and sort them by weight
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;
    });

    // Create a new graph to store the MST(Minimum Spanning Tree)
    Graph mst(this->name, this->directed, this->weighted_edges, this->weighted_nodes);
    
    // Copy all nodes to MST without edges and set the tree_id to 0
    for (auto node : this->get_nodes())
    {
        mst.add_node(node.id, node.weight);
        mst.get_node(node.id)->tree_id = 0;
    }

    // Counter for the added edges on MST
    size_t added_edges = 0;

    // Auxiliar variables
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    size_t tree_id_1, tree_id_2, new_tree_id=1;
    size_t n_nodes = mst.get_number_of_nodes();
    Edge edge;

    
    // Traverse all edges sorted and add to MST if it doesn't create a cycle
    while (added_edges < n_nodes - 1 && !edges.empty())
    {
        // Get the edge at front and remove it from the vector
        edge = edges[0];
        edges.erase(edges.begin());
        
        // Get the nodes of the edge
        node_1 = mst.get_node(edge.origin_id);
        node_2 = mst.get_node(edge.target_id);
        tree_id_1 = node_1->tree_id;
        tree_id_2 = node_2->tree_id;

        // If the nodes of the edge are not in the same subtree, add the edge to the MST
        if((tree_id_1 != tree_id_2) || (tree_id_1 == 0 || tree_id_2 == 0))
            add_edge_to_mst(mst, edge, tree_id_1, tree_id_2, new_tree_id, added_edges);
    }

    return mst;
}

Graph Graph::minimum_spanning_tree_by_prim()
{
    Graph mst(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    return mst;
}



//* ------------------------------------------
//* ------------Auxiliar Functions------------
//* ------------------------------------------

void add_edge_to_mst(Graph& mst, Edge& edge, size_t& tree_id_1, size_t& tree_id_2, size_t& new_tree_id, size_t& added_edges)
{
    mst.add_edge(edge.origin_id, edge.target_id, edge.weight);
    added_edges++;

    if(tree_id_1 == 0 && tree_id_2 == 0)
    {
        mst.get_node(edge.origin_id)->tree_id = new_tree_id;
        mst.get_node(edge.target_id)->tree_id = new_tree_id;
        new_tree_id++;
    }
    else if(tree_id_1 == 0)
        mst.get_node(edge.origin_id)->tree_id = tree_id_2;
    else if(tree_id_2 == 0)
        mst.get_node(edge.target_id)->tree_id = tree_id_1;
    else if(tree_id_1 != tree_id_2)
        for (auto node : mst.get_nodes())
        {
            if (node.tree_id == tree_id_2)
                node.tree_id = tree_id_1;
        }

}
