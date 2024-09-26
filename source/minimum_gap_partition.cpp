#include "../include/defines.hpp"
#include "../include/Graph.hpp"

Partitions Graph::mggpp_partition_greedy()
{
    const unsigned int p = this->number_of_partitions;
    if(p <= 1 || p >= this->number_of_nodes)
    {
        std::cout << "Invalid number of partitions" << std::endl;
        exit(1);
    }

    std::cout << "Number of partitions: " << p << std::endl;
    
    std::vector<Node> nodes = this->get_nodes();
    // std::sort(nodes.begin(), nodes.end(), [](Node& node_1, Node& node_2) -> bool {
    //     return node_1.number_of_edges < node_2.number_of_edges;
    // });

    Partitions clusters;
    this->partition_setup(nodes, clusters);


    auto gaps = std::vector<float>(p, inf_f);
    auto max_weight = std::vector<float>(p, 0.0);
    auto min_weight = std::vector<float>(p, inf_f);

    for(size_t i = 0; i < p; i++)
        for(size_t j = 0; j < clusters[i].size(); j++)
        {
            max_weight[i] = std::max(max_weight[i], clusters[i][j].weight);
            min_weight[i] = std::min(min_weight[i], clusters[i][j].weight);
        }

    // initialize gaps
    for(size_t i = 0; i < p; i++)
        gaps[i] = std::abs(clusters[i][0].weight - clusters[i][1].weight);

    // print clusters
    for(size_t i = 0; i < p; i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < clusters[i].size(); j++)
            std::cout << clusters[i][j].id << " ";
        std::cout << "\n";
    }

    size_t id;

    while(!nodes.empty())
    {
        float gap = inf_f;
        size_t gap_i = 0;
        bool found = false;
        
        id = this->greedy_aux(nodes[0], clusters, min_weight, max_weight, gap, gap_i);

        if(!found)
        {
            auto node = nodes[0];
            nodes.erase(nodes.begin());
            nodes.push_back(node);
            std::cout << "Node not found" << "\n";
            continue;
        }

        gaps[gap_i] = gap;
        clusters[gap_i].push_back(nodes[0]);
        nodes.erase(nodes.begin());
        // std::cout << "Node " << nodes[0].id << " added to cluster" << "\n";
        // nodes.erase(nodes.begin());
    }


    return clusters;
}

Partitions Graph::mggpp_greedy_randomized_adaptive(const unsigned int iterations, const float alpha)
{
    const unsigned int p = this->number_of_partitions;
    if(p <= 1 || p >= this->number_of_nodes)
    {
        std::cout << "Invalid number of partitions" << std::endl;
        exit(1);
    }
    if(alpha == 0 || alpha > 1)
    {
        std::cout << "Invalid alpha value" << std::endl;
        exit(1);
    }
    if(iterations == 0)
    {
        std::cout << "Invalid number of iterations" << std::endl;
        exit(1);
    }

    Partitions best_clusters;
    
    // for(size_t i = 0; i < iterations; i++)
    // {
    //     std::vector<Node> nodes = this->get_nodes();
    //     unsigned int nodes_size = nodes.size();

    //     // shuffle based on the alpha value
    //     std::random_shuffle(nodes.begin(), nodes.end(), [alpha, nodes_size](int) -> int {
    //         return (nodes_size-1) * alpha;
    //     });

    //     Partitions clusters;
    //     this->partition_setup(nodes, clusters);

    //     auto gaps = std::vector<float>(p, inf_f);

    //     // initialize gaps
    //     for(size_t i = 0; i < p; i++)
    //         gaps[i] = std::abs(clusters[i][0].weight - clusters[i][1].weight);

    //     while(!nodes.empty())
    //     {
    //         float gap = inf_f;
    //         size_t gap_i = 0;
    //         bool found = false;
            
    //         this->greedy_aux(nodes[0], clusters, found, gap, gap_i);

    //         if(!found)
    //         {
    //             auto node = nodes[0];
    //             nodes.erase(nodes.begin());
    //             nodes.push_back(node);
    //             continue;
    //         }

    //         gaps[gap_i] = gap;
    //         clusters[gap_i].push_back(nodes[0]);
    //     }

    //     if(i == 0)
    //         best_clusters = clusters;
    //     else
    //     {
    //         float best_gap = 0;
    //         float current_gap = 0;

    //         for(size_t i = 0; i < p; i++)
    //         {
    //             for(size_t j = 0; j < best_clusters[i].size(); j++)
    //                 best_gap += std::abs(best_clusters[i][0].weight - best_clusters[i][j].weight);
    //             for(size_t j = 0; j < clusters[i].size(); j++)
    //                 current_gap += std::abs(clusters[i][0].weight - clusters[i][j].weight);
    //         }

    //         if(current_gap < best_gap)
    //             best_clusters = clusters;
    //     }
        
        
    // }

    return best_clusters;

}


Partitions Graph::mggpp_greedy_randomized_adaptive_reactive()
{
    const unsigned int p = this->number_of_partitions;
    return Partitions(p);
}


//* ------------------------------------------------
//* ---------------Funções Auxiliares---------------
//* ------------------------------------------------
size_t Graph::greedy_aux(Node& node, Partitions& clusters, std::vector<float>& min, std::vector<float>& max, float& gap, size_t& gap_i)
{
    size_t id = -1;
    // // para cada cluster
    // for(size_t i = 0; i < clusters.size(); i++)
    // {
        // // para cada nó do cluster
        // for(size_t j = 0; j < clusters[i].size(); j++)
        // {
        //     if(this->connected(nodes[0].id, clusters[i][j].id) != 1)
        //         continue;

        //     found = true;
        //     // verifica se o gap é menor que o gap atual
        //     for(size_t k = 0; k < clusters[i].size(); k++)
        //         if(std::abs(nodes[0].weight - clusters[i][k].weight) < gap)
        //         {
        //             gap_i = i;
        //             gap = std::abs(nodes[0].weight - clusters[i][k].weight);
        //         }
        // }
    // }



    for(size_t i = 0; i < clusters.size(); i++)
    {
        if(this->connected(node.id, clusters[i][0].id) == 1)
        {
            if(node.weight >= min[i] && node.weight <= max[i])
            {
                gap = 
                id = i;
            }
        }
    }

    return id;
}

void Graph::partition_setup(std::vector<Node>& nodes, Partitions& clusters)
{
    for(size_t i = 0; i < this->number_of_partitions; i++)
    {
        clusters.push_back(std::vector<Node>());

        // add the node to the cluster
        clusters[i].push_back(nodes.front());
        nodes.erase(nodes.begin());
        size_t remove = 0;
        for(size_t j = 0; j < nodes.size(); j++)
        {
            Node node = nodes[j];
            float gap = inf_f;
            if(this->connected(node.id, clusters[i].front().id) == 1 && std::abs(node.weight - clusters[i][0].weight) < gap)
            {
                gap = node.weight - clusters[i].front().weight;
                remove = j;
            }
        }

        clusters[i].push_back(nodes[remove]);
        nodes.erase(nodes.begin() + remove);
    }
}
