#ifndef CUSTOM_PREFIX_TREE_H_INCLUDED
#define CUSTOM_PREFIX_TREE_H_INCLUDED

#include <string>
#include <vector>
#include <unistd.h>
#include "generic_vector.h"
#include <mutex>
#include <pthread.h>
#include <thread>
//#include <jthread>

#define MAX_THREADS 4

using namespace std;

struct nodes_vector;

typedef struct suffix_tree_node{
    //Qui viene salvato il prefisso presente in questo nodo dell'albero
    const char* suffix;

    //Size del suffisso
    int suffix_len;

    //Padre del nodo
    suffix_tree_node* father;

    //Array di indici nei quali è presente il prefisso di cui sopra
    int_vector* array_of_indexes;

    //Figli del nodo
    nodes_vector* sons;

    //Foglie del nodo, importante solo nei figli di root
    nodes_vector* leaves;

    //Una catena per ogni sotto albero, è importante solo nei figli di root
    //group_chains
    array_of_int_vector* chains_of_suffixes;

    //Chain of suffixes dal figlio di root al nodo
    int_vector* common_chain_of_suffiexes;

    //bit_vector del nodo
    bit_vector* bit_vec;

    //versione migiore del bit vector per common prefix merge
    common_elements_vector* common_elements_vec;

    //Lock per il nodo
    mutex node_lock;

}suffix_tree_node;

suffix_tree_node* build_suffix_tree_node(suffix_tree_node* father,const char* suffix,int suffix_len);
suffix_tree_node* build_suffix_tree_orphan_node(const char* suffix,int suffix_len);
void free_node(suffix_tree_node* root);
void free_node_2(suffix_tree_node* root);
suffix_tree_node* add_suffix_in_tree(suffix_tree_node* root,const char* suffix,int indice,int suffix_len);
suffix_tree_node* add_suffix_in_tree_2(suffix_tree_node* root,const char* suffix,int indice,int suffix_len);
suffix_tree_node* add_suffix_in_tree_3(suffix_tree_node* root,const char* suffix,int indice,int suffix_len);
suffix_tree_node* add_suffix_in_tree_4(suffix_tree_node* root,const char* suffix,int indice,int suffix_len);
void add_suffix_in_tree_4_multithreading(suffix_tree_node* root,const char* suffix,int indice,int suffix_len);
//Non fa i return(cosa bella)
void add_suffix_in_tree_5(suffix_tree_node* root,const char* suffix,int indice,int suffix_len,suffix_tree_node** res);
int16_t find_index_of_child_a_is_prefix_of_b(suffix_tree_node* node, const char* suffix);
void stampa_suffix_tree(suffix_tree_node* root);

typedef struct nodes_vector
    {
        suffix_tree_node** data;
        size_t size;
        size_t used;

    }nodes_vector;

nodes_vector* init_nodes_vector(size_t size);
bool add_in_nodes_vector(nodes_vector* x,suffix_tree_node* element);
bool init_chains_of_prefixes(suffix_tree_node* root,int size_of_the_word);
int LCP(char* w, int index1,int index2);
int LCP_with_given_strings(const char* x,const char* y);
int LCP_with_given_strings_2(const char* x,const char* y,int max_len);
int LCP_from_same_string(const char* piccola,const char* grande,int n);

//utils replicate
void quicksort_of_nodes_local(nodes_vector* x, int start, int end);
int binarySearch(suffix_tree_node* root, const char* x, int low, int high);
int binarySearch_with_redundancy(suffix_tree_node* root, const char* x, int suffix_len,int low, int high);
int binarySearch_2(suffix_tree_node* root, const char* x, int low, int high);
int binarySearch_2_with_redundancy(suffix_tree_node* root, const char* x,int suffix_len, int low, int high);
int binarySearch_3_with_redundancy(suffix_tree_node* root, const char* x,int suffix_len, int low, int high);
int binarySearch_4_with_redundancy(nodes_vector* n_vector, const char* x,int suffix_len, int low, int high,bool* is_equal);
int binarySearch_4_with_redundancy_2(nodes_vector* n_vector, const char* x,int suffix_len, int low, int high,bool** is_equal);
nodes_vector* add_in_order(nodes_vector* x,suffix_tree_node* node);
void add_in_order_2(nodes_vector* x,suffix_tree_node* node);
void add_in_order_3(nodes_vector* sons,suffix_tree_node* node,int starting_position);

void print_nodes_vector(nodes_vector* n_vec);

typedef struct alberello
{
    nodes_vector* roots;
    nodes_vector* leaves;
}alberello;

alberello* init_alberello();
//DA USARE SOLO QUANDO LA ROOTS È UGUALE ALLE FOGLIE
void join_two_alberelli(suffix_tree_node* a,suffix_tree_node* b);
void join_two_alberelli_2(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res);
void join_n_alberelli(suffix_tree_node** roots,int k,suffix_tree_node** res_tree);
void join_n_alberelli_multithreading(suffix_tree_node** roots,int k,suffix_tree_node** res_tree);
void add_in_alberello(alberello* alb,const char* suffix,int suffix_len);
void add_suffix_in_node_sons(suffix_tree_node* root,const char* suffix,int suffix_len);
void add_suffix_in_node_sons_2(suffix_tree_node* root,const char* suffix,int suffix_len,int index);
void add_node_in_node_sons(suffix_tree_node* opt_padre,suffix_tree_node* figlio);
void add_node_in_node_sons_2(suffix_tree_node* opt_padre,suffix_tree_node* figlio,int index,bool is_equal);
suffix_tree_node* search_father_for_suffix(suffix_tree_node* root,const char* suffix,int suffix_len);
suffix_tree_node* search_father_for_suffix_2(suffix_tree_node* root,const char* suffix,int suffix_len,int* index,bool* is_equal);
#endif