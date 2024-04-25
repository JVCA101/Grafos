#include "../include/Graph.hpp"


void Graph::basic_deep_search()
{
    // Marca todos os nós como não visitados
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        node->visited = false;

    // Chama a função recursiva
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
    {
        if(!node->visited)
            aux_basic_deep_search(node);
    }

}

void Graph::deep_search_connected_components(){
    // Marca todos os nós como não conectados
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
        node->connection_mark = 0;

    int component = 0;

    // Chama a função recursiva
    for(Node *node = this->first_node; node != nullptr; node = node->next_node)
    {
        if(node->connection_mark == 0)
        {
            component++;
            aux_deep_search_connected_components(node, component);
        }
    }
}

Graph Graph::subgraph_vertice_induced(const std::vector<Node> nodes)
{
    // Cria um novo grafo
    Graph subgraph(this->name + "_subgraph", this->directed, this->weighted_edges, this->weighted_nodes);

    // Adiciona os nós ao novo grafo
    for(auto node : nodes)
        subgraph.add_node(node.id, node.weight);

    // Adiciona as arestas entre os nós ao novo grafo
    for(Node *node = subgraph.first_node; node != nullptr; node = node->next_node)
        for(Node *aux = this->first_node; aux != nullptr; aux = aux->next_node)
            if(aux->id == node->id)
                for(Edge *edge = aux->first_edge; edge != nullptr; edge = edge->next_edge)
                    if(this->connected(aux->id, edge->target_id))
                        subgraph.add_edge(aux->id, edge->target_id, edge->weight);

    return subgraph;
}




//* Funções Auxiliares

void Graph::aux_remove_edge(Node *const node, const size_t node_position)
{
    Edge *prev_edge = nullptr;
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        if(edge->target_id == node_position)
        {
            // Caso seja a primeira aresta
            if(prev_edge == nullptr)
                node->first_edge = edge->next_edge;
            // Caso não seja a primeira aresta, atualiza o ponteiro da aresta anterior para o próximo	
            else
                prev_edge->next_edge = edge->next_edge;

            // Deleta a aresta
            delete edge;
            node->number_of_edges--;
            break;
        }

        prev_edge = edge;
    }
}

void Graph::aux_basic_deep_search(Node *const node)
{
    // Marca o nó como visitado
    node->visited = true;

    // Chama a função recursiva para os nós adjacentes
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        // Procura o nó
        Node* aux;
        for(aux = this->first_node; aux != nullptr; aux = aux->next_node)
        {
            if(aux->id == edge->target_id)
                break;
        }

        // Chama a função recursiva se o nó não foi visitado
        if(!aux->visited)
            aux_basic_deep_search(aux);
    }
}

void Graph::aux_deep_search_connected_components(Node *const node, const int connection_mark)
{
    // Marca o nó como visitado
    node->connection_mark = connection_mark;

    // Chama a função recursiva para os nós adjacentes
    for(Edge *edge = node->first_edge; edge != nullptr; edge = edge->next_edge)
    {
        // Procura o nó
        Node* aux;
        for(aux = this->first_node; aux != nullptr; aux = aux->next_node)
        {
            if(aux->id == edge->target_id)
                break;
        }

        // Chama a função recursiva se o nó não foi visitado
        if(aux->connection_mark == 0)
            aux_deep_search_connected_components(aux, connection_mark);
    }
}
