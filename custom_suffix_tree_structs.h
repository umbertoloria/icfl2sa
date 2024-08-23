#include <vector>
#include <map>

using namespace std;

typedef struct suffix_tree_node{
    //Qui viene salvato il prefisso presente in questo nodo dell'albero
    const char* suffix;

    //Size del suffisso
    int suffix_len;

    //Padre del nodo
    suffix_tree_node* father;

    //Array di indici nei quali è presente il prefisso di cui sopra
    std::vector<int> array_of_indexes;

    //Array di indici nei quali è presente il prefisso di cui sopra
    std::vector<int> custom_array_of_indexes;

    //Figli del nodo
    std::vector<suffix_tree_node*> sons;

    //Chain of suffixes dal figlio di root al nodo
    std::vector<int> common_chain_of_suffiexes;

    //bit_vector del nodo
    std::vector<bool> bit_vec;

    //posizione dove cambia l'array degli indici rispetto al padre
    int min_father=-1,max_father=-1;

    //da vedere
    std::map<int,std::vector<int>> index_to_nodes;

}suffix_tree_node;


typedef struct nodes_vector{
    suffix_tree_node** data;
    size_t size;
    size_t used;
}nodes_vector;

typedef struct alberello{
    nodes_vector* roots;
    nodes_vector* leaves;
}alberello;