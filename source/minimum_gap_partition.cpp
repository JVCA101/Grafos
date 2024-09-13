#include "../include/defines.hpp"
#include "../include/Graph.hpp"

using Partitions = std::vector<std::vector<Node>>;

std::vector<Node> Graph::mggpp_partition_greedy(const unsigned int p)
{
    if(p <= 1 || p >= this->number_of_nodes)
    {
        std::cout << "Invalid number of partitions" << std::endl;
        exit(1);
    }
    
    std::vector<Node> nodes = this->get_nodes();
    std::sort(nodes.begin(), nodes.end(), [](Node& node_1, Node& node_2) -> bool {
        // sort by degree
        return node_1.number_of_edges < node_2.number_of_edges;
    });

    Partitions partitions;
    for(size_t i = 0; i < p; i++)
        partitions.push_back(std::vector<Node>());

    for(auto& node : nodes)
        node.visited = false;

    unsigned int max_in_partition;
    {
        float num_max = nodes.size() / (p*2);
        max_in_partition = num_max + (nodes.size() % (p*2) != 0);
    }

    unsigned int cont = 0;
    Node node_1, node_2;

    while(!nodes.empty())
    {
        while (partitions[cont].size() < max_in_partition){

        node_1 = nodes.front();
        nodes.erase(nodes.begin());

        partitions[cont].push_back(node_1);

        
        for(size_t i = 0; i < nodes.size(); i++)
        {
            if (partitions[cont].size() == max_in_partition)
            {
                break;
            }
            
            std::vector<Node> nodes_degree_1;
            auto node = nodes[i];
            if(this->connected(node_1.id, node.id) == 1 && node.weight < node_2.weight)
            {
                node_2 = node;

                // verifica se o node_2 tem algum nó conectado a ele que tem grau 1
                for(auto& edge : this->get_edges(node_2.id))
                {
                    auto node_aux = this->get_node(edge.target_id);
                    if(node_aux->number_of_edges == 1)
                    {
                        nodes_degree_1.push_back(*node_aux);
                        break;
                    }
                }
                
                if(nodes_degree_1.size() <= max_in_partition - 2)
                {
                    // adiciona node_2 e os nós de grau 1 conectados a ele
                    partitions[cont].push_back(node_2);
                    for(auto& node : nodes_degree_1)
                        partitions[cont].push_back(node);

                    // remove os nós de grau 1 da lista de nós
                    for(auto& node : nodes_degree_1)
                    {
                        nodes.erase(std::find(nodes.begin(), nodes.end(), node));
                    }
                        nodes.erase(std::find(nodes.begin(), nodes.end(), node_2));

                } else {
                    // pula a interação do for
                    continue;
                }

            }
        }
        
        }

    }


}

std::vector<Node> Graph::mggpp_greedy_randomized_adaptive(const unsigned int p)
{

}


std::vector<Node> Graph::mggpp_greedy_randomized_adaptive_reactive(const unsigned int p)
{

}
