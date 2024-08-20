#include "include/Graph.hpp"
#include "defines.hpp"



int main(int argc, char* argv[])
{
    // if(argc < 6)
    // {
    //     std::cout << "Usage: " << argv[0] << " <input_file> <output_file> <Op_Direc> <Op_WeightEdge> <Op_WeightNode>" << std::endl;
    //     return 1;
    // }

    std::string input_file  = argv[1];
    std::string output_file = argv[2];
    // bool op_direc = std::stoi(argv[3]);
    // bool op_weight_edge = std::stoi(argv[4]);
    // bool op_weight_node = std::stoi(argv[5]);
    bool op_direc       = (argv[3] == "1") ? true : false;
    bool op_weight_edge = (argv[4] == "1") ? true : false;
    bool op_weight_node = (argv[5] == "1") ? true : false;

    std::ifstream file(input_file);

    Graph g(file);

    g.print_graph();

    // Graph graph(input_file, op_direc, op_weight_edge, op_weight_node);
    // graph.print_graph();

    return 0;
}
