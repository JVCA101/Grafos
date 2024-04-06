#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
    std::ifstream instance("5nU.dat");
    if(!instance.is_open())
    {
        std::cout << "Error: input file is not open\n";
        exit(1);
    }

    std::string line, string_node_1, string_node_2, string_weight;
    std::getline(instance, line);
    size_t num_nodes(0);
    num_nodes = std::stoi(line);
    std::cout << "Number of nodes: " << num_nodes << std::endl;

    size_t node_id_1, node_id_2;
    float weight;

    for(size_t i = 0; i < num_nodes; i++)
    {
        std::getline(instance, line);
        string_node_1 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_node_2 = line.substr(0, line.find(" "));
        line.erase(0, line.find(" ") + 1);
        string_weight = line;

        node_id_1 = std::stoi(string_node_1);
        node_id_2 = std::stoi(string_node_2);
        weight = std::stof(string_weight);

        std::cout << node_id_1 << " " << node_id_2 << " " << weight << std::endl;
    }

    
    return 0;
}
