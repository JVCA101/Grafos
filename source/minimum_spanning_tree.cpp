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
    std::vector<Edge> edges = this->get_edges();
    Edge edge_min = edges[0];

    Graph mst(this->name, this->directed, this->weighted_edges, this->weighted_nodes);

    for(auto node : this->get_nodes())
    {
        mst.add_node(node.id, node.weight);
        mst.get_node(node.id)->key = std::numeric_limits<float>::infinity();
        mst.get_node(node.id)->tree_id = 0;
    }
    
    mst.add_edge(edge_min.origin_id, edge_min.target_id, edge_min.weight);
    mst.get_node(edge_min.origin_id)->tree_id = 1;
    mst.get_node(edge_min.target_id)->tree_id = 1;
    
    float weight_origin, weight_target;

    for(auto node : mst.get_nodes())
    {
        if(node.tree_id)
            continue;

        weight_origin = weight_of_connection(*this, *mst.get_node(edge_min.origin_id), node);
        weight_target = weight_of_connection(*this, *mst.get_node(edge_min.target_id), node);

        if (weight_origin < weight_target){
            node.key = weight_origin;
            node.prox = mst.get_node(edge_min.origin_id);
        }
        else{
            node.key = weight_target;
            node.prox = mst.get_node(edge_min.target_id);
        }

       // node.key = std::min(weight_origin, weight_target);
    }

    size_t cont = 0;
    weight_origin = weight_target = 0;

    while(cont < mst.get_number_of_nodes() - 2)
    {
        Node *j = inicialize_j(mst);

        mst.add_edge(j->prox->id, j->id, j->key);
        j->tree_id = 1;
        j->key = 0;

        for(auto node : mst.get_nodes())
        {
            if(node.tree_id)
                continue;

            weight_origin = weight_of_connection(*this, *mst.get_node(j->id), node);
           
           if(weight_origin < node.key)
           {
               node.key = weight_origin;
               node.prox = mst.get_node(j->id);
           }

        }

        cont++;
    }

    
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

float weight_of_connection(Graph& graph, Node& node_1, Node& node_2)
{
    if(graph.connected(node_1.id, node_2.id))
        return graph.get_edge(node_1.id, node_2.id)->weight;

    return std::numeric_limits<float>::infinity();
}

Node* inicialize_j(Graph& graph)
{
    Node *j;

    // find j
    for(auto node : graph.get_nodes())
        if(node.tree_id != 0)
        {
            j = graph.get_node(node.id);
            break;
        }

    // find the smallest key
    for(auto node : graph.get_nodes())
        if(node.tree_id != 0 && node.key < j->key)
            j = graph.get_node(node.id);

    return j;
}
