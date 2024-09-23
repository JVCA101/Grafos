#include "include/Graph.hpp"
#include "defines.hpp"

int main(int argc, char const *argv[])
{
    std::string input_file  = argv[1];
    std::ifstream file(input_file);

    Graph g(file, { false, false, true });

    return 0;
}
