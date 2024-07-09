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
    result.reserve(father.size()+child.size());
    int temp_res;

    int i=0,j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    //cout<<"Comparing:\n";
    //printVec(father);
    //cout<<"\n";
    //printVec(child);
    //cout<<"\n";

    while( i<father.size() && j<child.size()){

        if(check_if_custom_index(icfl_list,strlen(S),father[i]) || check_if_custom_index(icfl_list,strlen(S),child[j]) ){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0){
                result.emplace_back(false);
                j++;
            }
            else{
                result.emplace_back(true);
                i++;
            }
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){
            result.emplace_back(true);
            i++;
        }
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){
            result.emplace_back(false);
            j++;
        }
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){
                result.emplace_back(true);
                i++;
            }
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){
                    result.emplace_back(false);
                    j++;
                }
                else{
                    result.emplace_back(true);
                    i++;
                }
            }
            else{
                if(father[i] > child[j]){
                    result.emplace_back(false);
                    j++;
                }
                else{
                    if(strcmp(S+child[j],S+father[i])<0){
                        result.emplace_back(false);
                        j++;
                    }
                    else{
                        result.emplace_back(true);
                        i++;
                    }
                }

            }
        }

    }

    while(j<child.size()){
        result.emplace_back(false);
        j++;
    }

    while(i<father.size()){
        result.emplace_back(true);
        i++;
    }



    return result;
}

std::vector<int> in_prefix_merge_bit_vector_6(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child){
    std::vector<int> result;
    result.reserve(father.size()+child.size());

    int i=0,j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && j<child.size()){
        if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){
            result.emplace_back(father[i]);
            i++;
        }
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){
            result.emplace_back(child[j]);
            j++;
        }
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){
                result.emplace_back(father[i]);
                i++;
            }
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    result.emplace_back(father[i]);
                    i++;
                }
            }
            else{
                if(father[i] > child[j]){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    if(strcmp(S+child[j],S+father[i])<0){
                        result.emplace_back(child[j]);
                        j++;
                    }
                    else{
                        result.emplace_back(father[i]);
                        i++;
                    }
                }

            }
        }

    }

    while(j<child.size()){
        result.emplace_back(child[j]);
        j++;
    }

    while(i<father.size()){
        result.emplace_back(father[i]);
        i++;
    }

    return result;
}

std::vector<int> in_prefix_merge_bit_vector_7(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child,vector<int> is_custom_suffix){
    std::vector<int> result;
    result.reserve(father.size()+child.size());

    int i=0,j=0,temp_res;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]]){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0){
                result.emplace_back(child[j]);
                j++;
            }
            else{
                result.emplace_back(father[i]);
                i++;
            }
        }
        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){
            result.emplace_back(father[i]);
            i++;
        }
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){
            result.emplace_back(child[j]);
            j++;
        }
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){
                result.emplace_back(father[i]);
                i++;
            }
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    result.emplace_back(father[i]);
                    i++;
                }
            }
            else{
                if(father[i] > child[j]){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    if(strcmp(S+child[j],S+father[i])<0){
                        result.emplace_back(child[j]);
                        j++;
                    }
                    else{
                        result.emplace_back(father[i]);
                        i++;
                    }
                }

            }
        }

    }

    while(j<child.size()){
        result.emplace_back(child[j]);
        j++;
    }

    while(i<father.size()){
        result.emplace_back(father[i]);
        i++;
    }

    return result;
}