#include "merge.h"

int get_factor(vector<int> icfl_list,int index){
    int max_factor=icfl_list.size()-1;

    for(int i=0;i<max_factor;i++){
        if(index >= icfl_list.at(i) && index < icfl_list.at(i+1)){
            return i;
        }
    }

    return max_factor;

}


std::vector<bool> in_prefix_merge_bit_vector_5(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child){
    std::vector<bool> result;

    int i=0,j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && j<child.size()){
        if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){
            result.push_back(true);
            i++;
        }
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){
            result.push_back(false);
            j++;
        }
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){
                result.push_back(true);
                i++;
            }
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){
                    result.push_back(false);
                    j++;
                }
                else{
                    result.push_back(true);
                    i++;
                }
            }
            else{
                if(father[i] > child[j]){
                    result.push_back(false);
                    j++;
                }
                else{
                    if(strcmp(S+child[j],S+father[i])<0){
                        result.push_back(false);
                        j++;
                    }
                    else{
                        result.push_back(true);
                        i++;
                    }
                }

            }
        }

    }

    while(j<child.size()){
        result.push_back(false);
        j++;
    }

    while(i<father.size()){
        result.push_back(true);
        i++;
    }

    return result;
}