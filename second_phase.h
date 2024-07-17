#include <iostream>
#include "generic_vector.h"
#include "custom_suffix_tree.h"
#include <string.h>
#include <vector>

using namespace std;

int get_factor(vector<int> icfl_list,int index);
int_vector* in_prefix_merge(const char* S, vector<int> icfl_list, int_vector* e, int_vector* g);

vector<int> get_chain_from_bit_vector_3(suffix_tree_node* root);
vector<int> join_int_vector_with_bit_vector_2(vector<int> father_chain,vector<int>& son_chain,vector<bool>& bit_vec);
void create_bit_vector_3_redundancy(const char* S,vector<int>& icfl_list,int icfl_list_size, suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);
void get_chain_from_root(const char* S,vector<int>& icfl_list,int icfl_list_size,suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);

void get_bit_vectors_from_root(const char* S,vector<int>& icfl_list,int icfl_list_size,suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);