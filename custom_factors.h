#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

typedef struct custom_index{
    long int index;
    bool is_original;
}custom_index;


void set_offset(int x);
int get_offset();
int get_first_offset();
vector<int> get_custom_factor(vector<int>& icfl,int word_size);
vector<int> get_custom_factor_random(vector<int>& icfl,int word_size);
vector<int> get_custom_factor_bigger_first_factor(vector<int>& icfl,int word_size);
bool check_if_custom_index(vector<int>& icfl,int word_size,int index);
bool check_if_custom_index_random(std::vector<int>& icfl,std::vector<int>& custom_icfl,int word_size,int index);
bool check_if_custom_index_bigger_first_factor(vector<int>& icfl,int word_size,int index);
bool check_if_normal_index(vector<int>& icfl,int word_size,int index);
bool check_if_normal_index_random(std::vector<int>& icfl,std::vector<int>& custom_icfl,int word_size,int index);
bool check_if_normal_index_bigger_first_factor(vector<int>& icfl,int word_size,int index);