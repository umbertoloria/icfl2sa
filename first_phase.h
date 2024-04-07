#include <malloc.h>
#include "utils.h"
#include <string.h>
#include <thread>

int get_max_size(vector<int> icfl_list,int lenght_of_word);

suffix_tree_node* creazione_albero_alberelli(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size);

void compute_i_phase(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i);
void compute_i_phase_multithreding(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i);
void compute_i_phase_alberello(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,alberello* alb,int i);
void compute_i_phase_alberello_2(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i);

void add_node_in_suffix_tree(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j);
void add_node_in_suffix_tree_multithreding(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j);
void add_node_in_suffix_tree_alberello(const char* S,vector<int> icfl_list,int icfl_size,alberello* alb,int i,int j);
void add_node_in_suffix_tree_alberello_2(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j);