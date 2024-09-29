#include "include/Graph.hpp"
#include "defines.hpp"

void print_partitions(Partitions partitions) noexcept
{
    auto gaps = std::vector<float>(partitions.size(), inf_f);
    for(size_t i = 0; i < partitions.size(); i++)
    {
        float max_weight = 0.0, min_weight = inf_f;
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < partitions[i].size(); j++)
        {
            max_weight = std::max(max_weight, partitions[i][j].weight);
            min_weight = std::min(min_weight, partitions[i][j].weight);
            std::cout << partitions[i][j].id << " ";
        }
        std::cout << "\n";
        gaps[i] = max_weight - min_weight;
    }

    std::cout << "Gaps: ";
    for(size_t i = 0; i < gaps.size(); i++)
        std::cout << gaps[i] << " ";
    std::cout << "\n";

    std::cout << "MGGPP: " << std::accumulate(gaps.begin(), gaps.end(), 0.0) << "\n";
}

struct Options
{
    bool greedy = false;
    bool greedy_randomized_adaptive = false;
    bool greedy_randomized_adaptive_reactive = false;
};


int main(int argc, char const *argv[])
{
    std::string input_file;
    Options options;
    
    // get options
    int opt;
    while((opt = getopt(argc, (char* const*)argv, "g:a:r:")) != -1)
    {
        switch(opt)
        {
            case 'g':
                options.greedy = true;
                input_file = optarg;
                break;
            case 'a':
                options.greedy_randomized_adaptive = true;
                input_file = optarg;
                break;
            case 'r':
                options.greedy_randomized_adaptive_reactive = true;
                input_file = optarg;
                break;
            default:
                std::cout << "Usage: " << argv[0] << " [-g|-a|-r] <input_file>\n";
                return 1;
        }
    }

    std::ifstream file(input_file);

    if(!file.is_open())
    {
        std::cout << "Error opening file\n";
        return 1;
    }

    Graph g(file, { false, false, true });

    if(options.greedy)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto partitions = g.mggpp_partition_greedy();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time: " << duration << "ms\n";

        print_partitions(partitions);
    }
    else if(options.greedy_randomized_adaptive)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto partitions = g.mggpp_greedy_randomized_adaptive(10000, 0.3);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time: " << duration << "ms\n";

        print_partitions(partitions);
    }
    else if(options.greedy_randomized_adaptive_reactive)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto partitions = g.mggpp_greedy_randomized_adaptive_reactive(10000, {0.05, 0.10, 0.15, 0.30, 0,50});
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time: " << duration << "ms\n";

        print_partitions(partitions);
    }

    return 0;
}
