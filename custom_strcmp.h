#include <vector>
#include <string.h>
#include "custom_suffix_tree_structs.h"
#include <iostream>

int custom_strcmp(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo);

//return 1 significa che viene inseito prima y, -1 prima x e 0 ancora non si sa.
int rules_custom_strcmp(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo);