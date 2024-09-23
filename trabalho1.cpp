#include "include/Graph.hpp"
#include "defines.hpp"

void print_menu() noexcept;
void print_dft(DFS_Tree& dft, Back_edges& back_edges) noexcept;
void save_dft(DFS_Tree& dft, Back_edges& back_edges, std::ofstream& output);

struct Results
{
    using sub_graph = std::vector<Node>;
    sub_graph dir_transitive_closure, inv_transitive_closure, dijkstra, articulation;
    Back_edges back_edges;
    float floyd;
    DFS_Tree depth_first_tree;
    Graph::Attributes attributes;
    Graph* prim, *kruskal;
};

int main(int argc, char* argv[])
{
    std::string input_file  = argv[1];
    std::string output_file = argv[2];
    bool op_direc       = (bool) std::stoi(argv[3]) && (argc > 3);
    bool op_weight_edge = (bool) std::stoi(argv[4]) && (argc > 4);
    bool op_weight_node = (bool) std::stoi(argv[5]) && (argc > 5);

    std::ifstream file(input_file);
    std::ofstream output(output_file);



    Graph g(file, op_direc, op_weight_edge, op_weight_node);

    g.print_graph();


    char save;
    int op{};
    size_t id{}, id2{};
    Results results;
    bool prim_called = false, kruskal_called = false;

    do
    {
        print_menu();

        std::cin >> op;

        switch(op)
        {
        case 1:
            std::cout << "Digite o id do nó: ";
            std::cin >> id;
            results.dir_transitive_closure = g.get_direct_transitive_closure(id);

            std::cout << "FECHO TRANSITIVO DIRETO: \n";
            for (auto& node : results.dir_transitive_closure)
            {
                std::cout << node.id << " ";
            }

            std::cout << "\nDeseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "FECHO TRANSITIVO DIRETO: \n";
                for (auto& node : results.dir_transitive_closure)
                {
                    output << node.id << " ";
                }
                output << "\n";
            }
            
            break;
        case 2:
            std::cout << "Digite o id do nó: ";
            std::cin >> id;
            results.inv_transitive_closure = g.get_inverse_transitive_closure(id);
            
            std::cout << "FECHO TRANSITIVO INDIRETO: \n";
            for (auto& node : results.inv_transitive_closure)
            {
                std::cout << node.id << " ";
            }
            std::cout << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "FECHO TRANSITIVO INDIRETO: \n";
                for (auto& node : results.inv_transitive_closure)
                {
                    output << node.id << " ";
                }
                output << "\n";
            }
            break;
        case 3:
            std::cout << "Digite o id do nó 1: ";
            std::cin >> id;
            std::cout << "Digite o id do nó 2: ";
            std::cin >> id2;
            results.dijkstra = g.shortest_path_dijkstra(id, id2);

            std::cout << "RESULTADO DIJKSTRA: \n";
            for (auto& node : results.dijkstra)
            {
                std::cout << node.id << " ";
            }
            std::cout << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "RESULTADO DIJKSTRA: \n";
                for (auto& node : results.dijkstra)
                {
                    output << node.id << " ";
                }
                output << "\n";
            }
            break;
        case 4:
            std::cout << "Digite o id do nó 1: ";
            std::cin >> id;
            std::cout << "Digite o id do nó 2: ";
            std::cin >> id2;
            results.floyd = g.shortest_path_floyd(id, id2);

            std::cout << "RESULTADO FLOYD: " << results.floyd << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "RESULTADO FLOYD: " << results.floyd << "\n";
            }
            break;
        case 5:
            if(prim_called)
                delete results.prim;
            results.prim = g.minimum_spanning_tree_by_prim();

            std::cout << "ÁRVORE GERADORA MÍNIMA POR PRIM:\n";
            results.prim->print_graph();
            std::cout << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "ÁRVORE GERADORA MÍNIMA POR PRIM:\n";
                results.prim->print_graph(output);
                output << "\n";
            }

            prim_called = true;
            break;
        case 6:
            if(kruskal_called)
                delete results.kruskal;
            results.kruskal = g.minimum_spanning_tree_by_kruskal();

            std::cout << "ÁRVORE GERADORA MÍNIMA POR KRUSKAL:\n";
            results.kruskal->print_graph();
            std::cout << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "ÁRVORE GERADORA MÍNIMA POR KRUSKAL:\n";
                results.kruskal->print_graph(output);
                output << "\n";
            }

            kruskal_called = true;
            break;
        case 7:
            std::cout << "Digite o id do nó: ";
            std::cin >> id;
            results.depth_first_tree = g.depth_first_tree(id, results.back_edges);

            print_dft(results.depth_first_tree, results.back_edges);

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
                save_dft(results.depth_first_tree, results.back_edges, output);
            break;
        case 8:
            results.attributes = g.get_attributes();

            std::cout << "RAIO: " << results.attributes.ray << "\n";
            std::cout << "DIÂMETRO: " << results.attributes.diameter << "\n";
            std::cout << "CENTRO: \n";
            for (auto& node : results.attributes.center)
                std::cout << node.id << " ";
            std::cout << "\n";
            std::cout << "PERIFERIA: \n";
            for (auto& node : results.attributes.periphery)
                std::cout << node.id << " ";
            std::cout << "\n";

            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "RAIO: " << results.attributes.ray << "\n";
                output << "DIÂMETRO: " << results.attributes.diameter << "\n";
                output << "CENTRO: \n";
                for (auto& node : results.attributes.center)
                    output << "  " << node.id << " ";
                output << "\n";
                output << "PERIFERIA: \n";
                for (auto& node : results.attributes.periphery)
                    output << "  " << node.id << " ";
                output << "\n";
            }
            break;
        case 9:
            results.articulation = g.articulation_points();

            std::cout << "PONTOS DE ARTICULAÇÃO: \n";
            for (auto& node : results.articulation)
                std::cout << node.id << " ";
            std::cout << "\n";
            
            std::cout << "Deseja salvar em arquivo? (s/n): ";
            std::cin >> save;
            if(save == 's')
            {
                output << "PONTOS DE ARTICULAÇÃO: \n";
                for (auto& node : results.articulation)
                    output << node.id << " ";
                output << "\n";
            }
            break;
        default:
            break;
        }

    } while (op != 0);

    if(prim_called)
        delete results.prim;
    if(kruskal_called)
        delete results.kruskal;

    return 0;
}

void print_menu() noexcept
{
    std::cout << "0 - Sair\n";
    std::cout << "1 - Fecho transitivo direto\n";
    std::cout << "2 - Fecho transitivo indireto\n";
    std::cout << "3 - Caminho mínimo entre dois nós usando Dijkstra\n";
    std::cout << "4 - Custo mínimo entre dois nós usando Floyd\n";
    std::cout << "5 - Árvore geradora mínima usando Prim\n";
    std::cout << "6 - Árvore geradora mínima usando Kruskal\n";
    std::cout << "7 - Árvore de busca em profundidade\n";
    std::cout << "8 - Raio, diâmetro, centro e periferia\n";
    std::cout << "9 - Pontos de articulação\n";
}

void print_dft(DFS_Tree& dft, Back_edges& back_edges) noexcept
{
    std::cout << "ÁRVORE DE BUSCA EM PROFUNDIDADE:\n";
    for (DFS_Tree::const_iterator i = dft.begin(); i != dft.end(); ++i)
    {
        std::cout << i->first << " -> ";
        for (auto& node : i->second)
        {
            std::cout << node << " ";
        }
        std::cout << "\n";
    }
    std::cout << "ARESTAS DE RETORNO:\n";
    for (auto& edge : back_edges)
    {
        std::cout << edge.first << " -> " << edge.second << "\n";
    }
    std::cout << "\n";
}

void save_dft(DFS_Tree& dft, Back_edges& back_edges, std::ofstream& output)
{
    output << "ÁRVORE DE BUSCA EM PROFUNDIDADE:\n";
    for (DFS_Tree::const_iterator i = dft.begin(); i != dft.end(); ++i)
    {
        output << i->first << " -> ";
        for (auto& node : i->second)
        {
            output << node << " ";
        }
        output << "\n";
    }
    output << "ARESTAS DE RETORNO:\n";
    for (auto& edge : back_edges)
    {
        output << edge.first << " -> " << edge.second << "\n";
    }
    output << "\n";
}