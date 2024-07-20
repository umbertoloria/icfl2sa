#include <map>
#include <iostream>
#include "first_phase.h"

typedef struct custom_prefix_trie{

    std::map<char,custom_prefix_trie> sons;
    //std::vector<custom_prefix_trie*> sons;
    suffix_tree_node* node;
    custom_prefix_trie* father;
    
}custom_prefix_trie;

custom_prefix_trie* init_custom_prefix_trie();
void add_in_custom_prefix_trie(custom_prefix_trie* root,const char* S,const char* suffix,int current_suffix_len,int suffix_len,int suffix_index,vector<int>& icfl_list,vector<int>& custom_icfl_list,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list);
void compute_i_phase_alberello_custom_prefix_trie(const char*S,int lenght_of_word,vector<int>& icfl_list,int icfl_size,vector<int>& custom_icfl_list,int custom_icfl_size,custom_prefix_trie* root,int i,vector<int>& is_custom_vec,vector<int>& factor_list);
custom_prefix_trie* creazione_albero_custom_prefix_trie(vector<int>& icfl_list,vector<int>& custom_icfl_list,const char* S,int lenght_of_word,int max_size,int custom_max_size,int n_threads);
void merge_custom_array_of_indexes_prefix_trie_recurive(const char* S,vector<int>& icfl_list,custom_prefix_trie* root,std::vector<int> &is_custom_suffix, std::vector<int> &factor_list);
void get_chain_from_root_2(const char* S,vector<int>& icfl_list,int icfl_list_size,custom_prefix_trie * root,std::vector<int>& father_vector,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);