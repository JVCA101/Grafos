#include "../include/Graph.hpp"


Graph Graph::minimum_spanning_tree_by_kruskal()
{   

    // Get all edges and sort them by weight
    std::vector<Edge> edges = this->get_edges();
    std::sort(edges.begin(), edges.end(), [](Edge& edge_1, Edge& edge_2) {
        return edge_1.weight < edge_2.weight;
    });

    // Create a new graph to store the MST(Minimum Spanning Tree)
    Graph mst(this->name, this->directed, this->weighted_edges, this->weighted_nodes);
    
    // Copy all nodes to MST without edges and set the tree_id to 1
    for (auto node : this->get_nodes())
    {
        mst.add_node(node.id, node.weight);
        mst.get_node(node.id)->tree_id = 1;
    }

    // Counter for the added edges on MST
    int added_edges = 0;

    // Auxiliar variables
    Node *node_1 = nullptr;
    Node *node_2 = nullptr;
    size_t tree_id_1=0, tree_id_2=0;
    Edge edge;

    
    // Traverse all edges sorted and add to MST if it doesn't create a cycle
    while (added_edges < mst.get_number_of_nodes() - 1)
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
        if (!tree_id_1 == 0 || !tree_id_2 == 0)
            add_edge_to_mst(mst, edge, tree_id_1, tree_id_2, added_edges);
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

void add_edge_to_mst(Graph& mst, Edge& edge, size_t& tree_id_1, size_t& tree_id_2, int& added_edges)
{
    mst.add_edge(edge.origin_id, edge.target_id, edge.weight);
    added_edges++;
    
    // If the nodes are not in any tree, create a new tree
    if (!tree_id_1 == 0 && !tree_id_2 == 0)
    {
        tree_id_1 = mst.get_number_of_nodes();
        tree_id_2 = mst.get_number_of_nodes();
    }

    // If one of the nodes is in a tree, add the other node to the same tree
    else if (tree_id_1 == 0)
    {
        tree_id_1 = tree_id_2;
    }
    else if (tree_id_2 == 0)
    {
        tree_id_2 = tree_id_1;
    }
    // If both nodes are in different trees, merge the trees
    else
    {
        for (auto node : mst.get_nodes())
            if (node.tree_id == tree_id_2)
                node.tree_id = tree_id_1;
    }
}
