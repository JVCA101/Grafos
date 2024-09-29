#include "include/Graph.hpp"
#include "defines.hpp"

void print_partitions(Partitions partitions) noexcept
{
    float total_weight = 0.0;
    for(size_t i = 0; i < partitions.size(); i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < partitions[i].size(); j++)
        {
            total_weight += partitions[i][j].weight;
            std::cout << partitions[i][j].id << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Total weight: " << total_weight << "\n";
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
        auto partitions = g.mggpp_greedy_randomized_adaptive(1000, 0.3);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time: " << duration << "ms\n";

        print_partitions(partitions);
    }
    else if(options.greedy_randomized_adaptive_reactive)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto partitions = g.mggpp_greedy_randomized_adaptive_reactive(1000, {0.05, 0.10, 0.15, 0.30, 0,50});
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Execution time: " << duration << "ms\n";

        print_partitions(partitions);
    }

    return 0;
}
