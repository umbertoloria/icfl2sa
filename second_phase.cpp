#include <iostream>
#include "generic_vector.h"
#include "custom_suffix_tree.h"
#include <string.h>
#include <vector>
#include "second_phase.h"
#include "merge.h"
#include "utils.h"

using namespace std;

vector<int> join_int_vector_with_bit_vector(vector<int> father_chain,vector<int> son_chain,vector<bool> bit_vec){
    //i=indice array dei suffissi del nodo padre
    //j=indice array dei suffissi del nodo figlio
    //z=indice bit vector del nodo figlio
    int i,j;
    i=j=0;
    vector<int> result;
    for(int z=0;z<bit_vec.size();z++){
        if(bit_vec[z]) result.push_back(father_chain[i++]);
        else result.push_back(son_chain[j++]);
    }
    return result;
}

//O usiamo qusta ovunque, oppure no
//Fa affidamento sul fatto che gli array vengono duplicati quindi non si possono fare determinate assunzioni
//Tipo fare attenzione a quando si usa la free()
vector<int> get_chain_from_bit_vector_3(suffix_tree_node* root){
    if(root->father==NULL) return root->array_of_indexes;
    if(!root->common_chain_of_suffiexes.empty()) return root->common_chain_of_suffiexes;
    return join_int_vector_with_bit_vector(get_chain_from_bit_vector_3(root->father),root->array_of_indexes,root->bit_vec);
}

void create_bit_vector_3(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root){

    root->bit_vec=in_prefix_merge_bit_vector_5(S,icfl_list,icfl_list_size,get_chain_from_bit_vector_3(root->father),root->array_of_indexes);
}


void create_bit_vector_3_redundancy(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root){
    //clock_t tot_inprefixmerge=0,tot_getchainfrombitvector=0,tStart;
    //tStart = clock();
    root->bit_vec=in_prefix_merge_bit_vector_5(S,icfl_list,icfl_list_size,get_chain_from_bit_vector_3(root->father),root->array_of_indexes);
    //printf("in_prefix_merge_bit_vector_3 Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    //tStart = clock();
    root->common_chain_of_suffiexes = get_chain_from_bit_vector_3(root);
    //printf("get_chain_from_bit_vector_3 Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

//Utilizza i common elements
//void create_bit_vector_4(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root){
//
//    int_vector* father_chain = get_chain_from_common_elements_vector(root->father);
//    //cout<<"\nottenuta catena del padre\n";
//    root->common_elements_vec=in_prefix_merge_bit_vector_5(S,icfl_list,icfl_list_size,father_chain,root->array_of_indexes);
//
//}


void get_bit_vectors_from_root(const char* S,vector<int> icfl_list,int icfl_list_size,suffix_tree_node* root){
    create_bit_vector_3(S,icfl_list,icfl_list_size,root);
    for(int i=0;i<root->sons.size();i++) 
        get_bit_vectors_from_root(S,icfl_list,icfl_list_size,root->sons[i]);
    return;
}


