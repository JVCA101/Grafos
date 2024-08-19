#include "../include/Graph.hpp"

std::vector<Node> Graph::shortest_path_dijkstra(size_t node_id_1, size_t node_id_2)
{
    //TODO terminar
    std::vector<Node> S_barra;
    std::vector<Node> S;
    std::vector<float> pi;

    for(size_t i = 0; i < number_of_nodes; i++)
    {
        if(i == node_id_1)
        {
            S.push_back(*get_node(i));
            pi.push_back(0);
        }
        // else if()
        // {
        //     pi.push_back()
        // }
        else
        {
            S_barra.push_back(*get_node(i));
            pi.push_back(inf_f);
        }
    }


    while(S_barra.size() > 0)
    {
        // Node* u = nullptr;
        float min = inf_f;
        size_t id_min = INFINITY;
        size_t counter = 0;
        size_t index = 0;

        for(auto node : S_barra)
        {
            if(pi[node.id] < min)
            {
                min = pi[node.id];
                id_min = node.id;
                index = counter;
            }
            counter++;
        }

        float min = pi[id_min];
        Node *u = get_node(id_min);

        // S.push_back(*u);
        S_barra.erase(S_barra.begin() + index);


        // S.push_back(*u);
        // S_barra.erase(std::remove(S_barra.begin(), S_barra.end(), *u), S_barra.end());

        // for(auto edge : u->get_edges())
        // {
        //     Node *v = get_node(edge.get_node_id());
        //     if(pi[v->get_id()] > pi[u->get_id()] + edge.get_weight())
        //     {
        //         pi[v->get_id()] = pi[u->get_id()] + edge.get_weight();
        //     }
        // }
    }


}

float Graph::shortest_path_floyd(size_t node_id_1, size_t node_id_2)
{
    //TODO
    auto nodes = this->get_nodes();
    size_t n = this->number_of_nodes;
    float** A = new float*[n];
    for(size_t i = 0; i < n; ++i)
        A[i] = new float[n];
    Edge* edge;

    size_t node_1 = index_of_node(node_id_1);
    size_t node_2 = index_of_node(node_id_2);

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

GraphAttributes  Graph::get_graph_atributes(Graph& graph)
{
    float** matrix = graph.shortest_path_floyd_matrix();
    // number of nodes
    size_t n = graph.get_number_of_nodes();
    // minimal value of matrix
    float ray = 0;
    // maximal value of matrix
    float diameter = 0;
    
    for (size_t i; i < n; i++)
    {
        for (size_t j; j < n; j++)
        {
            if (matrix[i][j] > diameter)
                diameter = matrix[i][j];
            if (matrix[i][j] < ray)
                ray = matrix[i][j];
        }
    }

    std::vector<Node> periphery;
    std::vector<Node> center;


    for (size_t i; i < n; i++)
    {
        for (size_t j; j < n; j++){
            if (matrix[i][j] == ray){
                center.push_back(*graph.get_node(i));
            } else if (matrix[i][j] == diameter){
                periphery.push_back(*graph.get_node(i));
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
