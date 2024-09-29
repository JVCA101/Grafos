#include "include/Graph.hpp"
#include "defines.hpp"

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string input_file  = argv[1];
    std::ifstream file(input_file);

    Graph g(file, { false, false, true });

    auto partitions = g.mggpp_greedy_randomized_adaptive_reactive(10000, { 0.1, 0.2, 0.3, 0.4, 0.5 });

    // compute gaps
    auto gaps = std::vector<float>();

    for(auto& cluster : partitions)
    {
        float max_weight = 0.0;
        float min_weight = inf_f;

        for(auto& node : cluster)
        {
            max_weight = std::max(max_weight, node.weight);
            min_weight = std::min(min_weight, node.weight);
        }

        gaps.push_back(max_weight - min_weight);
    }

    for(size_t i = 0; i < partitions.size(); i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < partitions[i].size(); j++)
            std::cout << partitions[i][j].id << " ";
        std::cout << "\n";
    }

    // print gaps
    float total_gap = 0;
    for(size_t i = 0; i < partitions.size(); i++){
        std::cout << "Gap " << i << ": " << gaps[i] << "\n";
        total_gap += gaps[i];
    }

    std::cout << "Total gap: " << total_gap << "\n";

    // for(size_t i = 0; i < partitions.size(); i++)
    // {
    //     std::cout << "Cluster " << i << ": ";
    //     for(size_t j = 0; j < partitions[i].size(); j++)
    //         std::cout << partitions[i][j].id << " ";
    //     std::cout << "\n";
    // }

    return 0;
}
