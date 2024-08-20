#include "include/Graph.hpp"
#include "defines.hpp"



int main(int argc, char* argv[])
{
    std::string input_file  = argv[1];
    std::string output_file = argv[2];
    bool op_direc       = (bool) std::stoi(argv[3]);
    bool op_weight_edge = (bool) std::stoi(argv[4]);
    bool op_weight_node = (bool) std::stoi(argv[5]);

    std::ifstream file(input_file);
    std::ofstream output(output_file);



    Graph g(file, op_direc, op_weight_edge, op_weight_node);

    g.print_graph(output);

    return 0;
}
