#include <malloc.h>
#include "utils.h"
#include <string.h>

char** get_lyndon_words(string* word,vector<int> icfl_list);
int get_max_size(vector<int> icfl_list,int lenght_of_word);
suffix_tree_node* creazione_albero(char** list_of_lyndon_words,vector<int> icfl_list,const char* S,int lenght_of_word,int max_size);
suffix_tree_node* creazione_albero_2(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size);
suffix_tree_node* creazione_albero_3(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size);