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

    auto partitions = g.mggpp_partition_greedy();

    for(size_t i = 0; i < partitions.size(); i++)
    {
        std::cout << "Cluster " << i << ": ";
        for(size_t j = 0; j < partitions[i].size(); j++)
            std::cout << partitions[i][j].id << " ";
        std::cout << "\n";
    }

    return 0;
}
