#include "generic_vector.h"
#include "utils.h"
#include "custom_factors.h"
#include <vector>
#include <string.h>

std::vector<int> get_factor_list(vector<int>& icfl_list,int str_len);

std::vector<bool> in_prefix_merge_bit_vector_5(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int> father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix);
std::vector<bool> in_prefix_merge_bit_vector_5_1(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int> father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix,vector<int>& factor_list);
std::vector<int> in_prefix_merge_bit_vector_5_2(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix,vector<int>& factor_list);
std::vector<int> in_prefix_merge_bit_vector_5_3(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);
void in_prefix_merge_bit_vector_5_4(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);
void in_prefix_merge_bit_vector_5_5(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);
std::vector<int> in_prefix_merge_bit_vector_6(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child);
std::vector<int> in_prefix_merge_bit_vector_7(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child,vector<int> is_custom_suffix);
void in_prefix_merge_bit_vector_8(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, int child,vector<int>& is_custom_suffix,int father_lenght,vector<int>& factor_list);
std::vector<int> in_prefix_merge_bit_vector_9(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, vector<int>& child,vector<int>& is_custom_suffix,int father_lenght,vector<int>& factor_list);

void alternative_prefix_merge_bit_vector(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);
void alternative_prefix_merge_bit_vector_2(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list);