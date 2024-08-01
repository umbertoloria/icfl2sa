#include <iostream>
#include <string.h>
#include <vector>
#include "second_phase.h"
#include "custom_prefix_trie.h"

using namespace std;

std::vector<int> common_prefix_merge_4_multithreading_2(std::vector<int> x,std::vector<int> y, std::vector<int> common_elements);
vector<int> get_common_prefix_merge_4_multihreading_2(suffix_tree_node* root);

std::vector<int> common_prefix_merge_4_multithreading_3(std::vector<int>& x,std::vector<int>& y, std::vector<int>& common_elements);
std::vector<int> get_common_prefix_merge_4_multihreading_3(custom_prefix_trie* root,std::vector<int>& common);