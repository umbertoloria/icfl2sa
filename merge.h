#include "generic_vector.h"
#include "utils.h"
#include "custom_factors.h"
#include <vector>
#include <string.h>

std::vector<bool> in_prefix_merge_bit_vector_5(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child);
std::vector<int> in_prefix_merge_bit_vector_6(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child);
std::vector<int> in_prefix_merge_bit_vector_7(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child,vector<int> is_custom_suffix);
void in_prefix_merge_bit_vector_8(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int>& father, int child,vector<int> is_custom_suffix);