#include <vector>
#include "utils.h"

using namespace std;

typedef struct custom_index{
    long int index;
    bool is_original;
}custom_index;



vector<int> get_custom_factor(vector<int>& icfl,int word_size);
bool check_if_custom_index(vector<int>& icfl,int word_size,int index);
bool check_if_normal_index(vector<int>& icfl,int word_size,int index);