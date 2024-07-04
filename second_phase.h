#include <iostream>
#include "generic_vector.h"
#include "custom_suffix_tree.h"
#include <string.h>
#include <vector>

using namespace std;

int get_factor(vector<int> icfl_list,int index);
int_vector* in_prefix_merge(const char* S, vector<int> icfl_list, int_vector* e, int_vector* g);

vector<int> get_chain_from_bit_vector_3(suffix_tree_node* root);
void create_bit_vector_3_redundancy(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root);

void get_bit_vectors_from_root(const char* S,vector<int> icfl_list,int icfl_list_size,suffix_tree_node* root);