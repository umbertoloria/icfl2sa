#include "custom_suffix_tree.h"
#include <map>
#include <iostream>

typedef struct custom_prefix_trie{
    std::map<char,custom_prefix_trie> sons;
    suffix_tree_node* node;
}custom_prefix_trie;

custom_prefix_trie init_custom_prefix_trie();
void add_in_custom_prefix_trie(custom_prefix_trie* root,const char* suffix,int current_suffix_len,int suffix_len,int suffix_index);
void compute_i_phase_alberello_custom_prefix_trie(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,custom_prefix_trie * root,int i);
void add_node_in_suffix_tree_alberello_custom_prefix_trie(const char* S,vector<int> icfl_list,int icfl_size,custom_prefix_trie * root,int i,int j);
custom_prefix_trie creazione_albero_custom_prefix_trie(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size);