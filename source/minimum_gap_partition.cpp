#include "../include/defines.hpp"
#include "../include/Graph.hpp"
#include <algorithm>
#include <random>

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

    // print gaps
    for(size_t i = 0; i < p; i++)
        std::cout << "Gap " << i << ": " << gaps[i] << "\n";

    while(!nodes.empty())
    {
        float gap = inf_f;
        size_t gap_i = 0;
        bool found = false;

        size_t id = this->greedy_aux(nodes[0], clusters, min_weight, max_weight, gap, gap_i);

        if(id == -1)
        {
            auto node = nodes[0];
            nodes.erase(nodes.begin());
            nodes.push_back(node);
            std::cout << "id_node: " << node.id << "\nNode not found" << "\n";
            continue;
        }

        if (max_weight[id] - nodes[0].weight < nodes[0].weight - min_weight[id]){
            max_weight[id] = nodes[0].weight;
        } else {
            min_weight[id] = nodes[0].weight;
        }

        gaps[id] = max_weight[id] - min_weight[id];
        clusters[id].push_back(nodes[0]);
        nodes.erase(nodes.begin());

        // gaps[gap_i] = gap;
        // clusters[gap_i].push_back(nodes[0]);
        // nodes.erase(nodes.begin());
        // std::cout << "Node " << nodes[0].id << " added to cluster" << "\n";
        // nodes.erase(nodes.begin());
    }

    // print gaps
    float total_gap = 0;
    for(size_t i = 0; i < p; i++){
        std::cout << "Gap " << i << ": " << gaps[i] << "\n";
        total_gap += gaps[i];
    }
    std::cout << "Total gap: " << total_gap << "\n";


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
    std::vector<float> best_gaps;
    
    for(size_t k = 0; k < iterations; k++)
    {
        std::vector<Node> nodes = this->get_nodes();
        unsigned int nodes_size = nodes.size();

        // shuffle based on the alpha value
        // std::random_shuffle(nodes.begin(), nodes.end(), [alpha, nodes_size](int) -> int {
        //     return (nodes_size-1) * alpha;
        // });

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(nodes.begin(), nodes.end(), g);

        if (alpha < 1.0) {
            int n_swaps = nodes.size() * alpha;  // Reducing the number of swaps based on alpha
            for (int i = 0; i < n_swaps; ++i) {
                std::swap(nodes[i], nodes[nodes.size() - i - 1]);
            }
        }


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

        while(!nodes.empty())
        {
            float gap = inf_f;
            size_t gap_i = 0;
            bool found = false;

            size_t id = this->greedy_aux(nodes[0], clusters, min_weight, max_weight, gap, gap_i);

            if(id == -1)
            {
                auto node = nodes[0];
                nodes.erase(nodes.begin());
                nodes.push_back(node);
                // std::cout << "id_node: " << node.id << "\nNode not found" << "\n";
                continue;
            }

            if (max_weight[id] - nodes[0].weight < nodes[0].weight - min_weight[id]){
                max_weight[id] = nodes[0].weight;
            } else {
                min_weight[id] = nodes[0].weight;
            }

            gaps[id] = max_weight[id] - min_weight[id];
            clusters[id].push_back(nodes[0]);
            nodes.erase(nodes.begin());
        }

        if(k == 0)
        {
            best_clusters = clusters;
            best_gaps = gaps;
        }
        else
        {
            float best_gap = 0;
            float current_gap = 0;

            for(size_t i = 0; i < p; i++)
            {
                for(size_t j = 0; j < best_clusters[i].size(); j++)
                    best_gap += std::abs(best_clusters[i][0].weight - best_clusters[i][j].weight);
                for(size_t j = 0; j < clusters[i].size(); j++)
                    current_gap += std::abs(clusters[i][0].weight - clusters[i][j].weight);
            }

            if(current_gap < best_gap)
            {
                best_clusters = clusters;
                best_gaps = gaps;
            }
        }
        
        
    }

    // print clusters
    for(size_t i = 0; i < p; i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < best_clusters[i].size(); j++)
            std::cout << best_clusters[i][j].id << " ";
        std::cout << "\n";
    }

    return best_clusters;

}


Partitions Graph::mggpp_greedy_randomized_adaptive_reactive(const unsigned int iterations,const std::vector<float> alphas)
{
    const unsigned int p = this->number_of_partitions;
    if(p <= 1 || p >= this->number_of_nodes)
    {
        std::cout << "Invalid number of partitions" << std::endl;
        exit(1);
    }

    // Iniciar com uma lista de alphas e suas probabilidades
    std::vector<float> alpha_scores(alphas.size(), 0.0f); // armazena a qualidade de cada alpha
    std::vector<float> alpha_probabilities(alphas.size(), 1.0f / alphas.size()); // probabilidade inicial uniforme

    Partitions best_clusters;
    std::vector<float> best_gaps;

    std::random_device rd;
    std::mt19937 g(rd());

    for(size_t k = 0; k < iterations; k++)
    {
        // Escolher um alpha baseado em suas probabilidades
        std::discrete_distribution<int> distribution(alpha_probabilities.begin(), alpha_probabilities.end());
        int selected_alpha_index = distribution(g);
        float alpha = alphas[selected_alpha_index];

        // Processar a partição com o alpha selecionado
        std::vector<Node> nodes = this->get_nodes();
        unsigned int nodes_size = nodes.size();

        std::shuffle(nodes.begin(), nodes.end(), g);
        if (alpha < 1.0) {
            int n_swaps = nodes.size() * alpha;
            for (int i = 0; i < n_swaps; ++i) {
                std::swap(nodes[i], nodes[nodes.size() - i - 1]);
            }
        }

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

        for(size_t i = 0; i < p; i++)
            gaps[i] = std::abs(clusters[i][0].weight - clusters[i][1].weight);

        while(!nodes.empty())
        {
            float gap = inf_f;
            size_t gap_i = 0;

            size_t id = this->greedy_aux(nodes[0], clusters, min_weight, max_weight, gap, gap_i);

            if(id == -1)
            {
                auto node = nodes[0];
                nodes.erase(nodes.begin());
                nodes.push_back(node);
                continue;
            }

            if (max_weight[id] - nodes[0].weight < nodes[0].weight - min_weight[id]){
                max_weight[id] = nodes[0].weight;
            } else {
                min_weight[id] = nodes[0].weight;
            }

            gaps[id] = max_weight[id] - min_weight[id];
            clusters[id].push_back(nodes[0]);
            nodes.erase(nodes.begin());
        }

        // Avaliar a solução atual e comparar com a melhor solução
        float current_gap = 0;
        float best_gap = 0;

        for(size_t i = 0; i < p; i++)
        {
            for(size_t j = 0; j < best_clusters[i].size(); j++)
                best_gap += std::abs(best_clusters[i][0].weight - best_clusters[i][j].weight);
            for(size_t j = 0; j < clusters[i].size(); j++)
                current_gap += std::abs(clusters[i][0].weight - clusters[i][j].weight);
        }

        if(k == 0 || current_gap < best_gap)
        {
            best_clusters = clusters;
            best_gaps = gaps;
        }

        // Atualizar o score do alpha escolhido com base na qualidade da solução
        alpha_scores[selected_alpha_index] += best_gap - current_gap;

        // Atualizar as probabilidades de alpha com base nos scores
        float total_score = std::accumulate(alpha_scores.begin(), alpha_scores.end(), 0.0f);
        for(size_t i = 0; i < alpha_probabilities.size(); i++)
        {
            alpha_probabilities[i] = (alpha_scores[i] + 1.0f) / (total_score + alphas.size());
        }
    }

    // print clusters
    for(size_t i = 0; i < p; i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < best_clusters[i].size(); j++)
            std::cout << best_clusters[i][j].id << " ";
        std::cout << "\n";
    }

    return best_clusters;
}



//* ------------------------------------------------
//* ---------------Funções Auxiliares---------------
//* ------------------------------------------------
size_t Graph::greedy_aux(Node& node, Partitions& clusters, std::vector<float>& min, std::vector<float>& max, float& gap, size_t& gap_i)
{
    size_t id = -1;

    size_t better_gap = inf_f;


    for(size_t i = 0; i < clusters.size(); i++)
    {
        for ( size_t j = 0; j < clusters[i].size(); j++){

            if(this->connected(node.id, clusters[i][j].id) == 1)
            {
                // std::cout << "Node " << node.id << " is connected to cluster " << i << "\n";
                if(node.weight >= min[i] && node.weight <= max[i])
                {
                    return i;
                }
                else if(node.weight < min[i]){
                    if(max[i] - node.weight < better_gap)
                    {
                        better_gap = max[i] - node.weight;
                        id = i;
                    }
                }
                else if(node.weight > max[i]){
                    if(node.weight - min[i] < better_gap)
                    {
                        better_gap = node.weight - min[i];
                        id = i;
                    }
                }
                break;
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
        float gap = inf_f;
        for(size_t j = 0; j < nodes.size(); j++)
        {
            Node node = nodes[j];
            // float gap = inf_f;
            if(this->connected(node.id, clusters[i].front().id) == 1 && std::abs(node.weight - clusters[i][0].weight) < gap)
            {
                gap = node.weight - clusters[i].front().weight;
                remove = j;
            }
        }

        clusters[i].push_back(nodes[remove]);
        nodes.erase(nodes.begin() + remove);
    }

    // // print clusters
    // for(size_t i = 0; i < this->number_of_partitions; i++)
    // {
    //     std::cout << "Cluster " << i << ": ";
    //     for(size_t j = 0; j < clusters[i].size(); j++)
    //         std::cout << clusters[i][j].id << " ";
    //     std::cout << "\n";
    // }
}
