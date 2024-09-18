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
    // std::sort(nodes.begin(), nodes.end(), [](Node& node_1, Node& node_2) -> bool {
    //     return node_1.number_of_edges < node_2.number_of_edges;
    // });

    Partitions clusters;
    for(size_t i = 0; i < p; i++)
    {
        clusters.push_back(std::vector<Node>());

        // add the node to the cluster
        clusters[i].push_back(nodes.front());
        nodes.erase(nodes.begin());
        Node node;
        size_t remove = 0;
        for(size_t j = 0; j < nodes.size(); j++)
        {
            node = nodes[j];
            float gap = inf_f;

            if(this->connected(node.id, clusters[i][j].id) == 1 && std::abs(node.weight - clusters[i][j].weight) < gap)
            {
                gap = node.weight - clusters[i].front().weight;
                remove = j;
            }
        }

        clusters[i].push_back(node);
        nodes.erase(nodes.begin() + remove);
    }

    auto gaps = std::vector<float>(p, inf_f);

    // initialize gaps
    for(size_t i = 0; i < p; i++)
        gaps[i] = std::abs(clusters[i][0].weight - clusters[i][1].weight);


    while(!nodes.empty())
    {
        float gap = inf_f;
        size_t gap_i = 0;
        bool found = false;
        // para cada cluster
        for(size_t i = 0; i < clusters.size(); i++)
        {
            // para cada nó do cluster
            for(size_t j = 0; j < clusters[i].size(); j++)
            {
                if(this->connected(nodes[0].id, clusters[i][j].id) != 1)
                    continue;

                found = true;
                // verifica se o gap é menor que o gap atual
                for(size_t k = 0; k < clusters[i].size(); k++)
                    if(std::abs(nodes[0].weight - clusters[i][k].weight) < gap)
                    {
                        gap_i = i;
                        gap = std::abs(nodes[0].weight - clusters[i][k].weight);
                    }
            }
        }

        if(!found)
        {
            auto node = nodes[0];
            nodes.erase(nodes.begin());
            nodes.push_back(node);
            continue;
        }

        gaps[gap_i] = gap;
        clusters[gap_i].push_back(nodes[0]);
    }

}

std::vector<Node> Graph::mggpp_greedy_randomized_adaptive(const unsigned int p)
{

}


std::vector<Node> Graph::mggpp_greedy_randomized_adaptive_reactive(const unsigned int p)
{

}
