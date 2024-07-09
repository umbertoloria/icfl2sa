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
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0) result.emplace_back(child[j++]);
            else result.emplace_back(father[i++]);
        }
        //A questo punto solo uno o nessuno dei due è arificiale
        //Se il primo è artificiale allora si inverte la regola
        else if (is_custom_suffix[father[i]]){
            if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) result.emplace_back(child[j++]);
            else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])) result.emplace_back(father[i++]);
            else{
                if(father[i] >= icfl_list[icfl_list_size-1]) result.emplace_back(child[j++]);
                else if(child[j] >= icfl_list[icfl_list_size-1]){
                    if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(father[i++]);
                    else result.emplace_back(child[j++]);
                }
                else{
                    if(father[i] > child[j]) result.emplace_back(father[i++]);
                    else{
                        //Questo uguale all'originale, c'è la strcmp 
                        if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                        else result.emplace_back(father[i++]);
                    }

                }
            }
        }
        //Se nessuno dei due è custom:
        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) result.emplace_back(father[i++]);
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])) result.emplace_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]) result.emplace_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                else result.emplace_back(father[i++]);
            }
            else{
                if(father[i] > child[j]) result.emplace_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                    else result.emplace_back(father[i++]);
                }

            }
        }

    }

    while(j<child.size()) result.emplace_back(child[j++]);

    while(i<father.size()) result.emplace_back(father[i++]);

    return result;
}

void in_prefix_merge_bit_vector_8(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int>& father, int child,vector<int> is_custom_suffix){

    int i=0,j=0,temp_res;
    bool flag=true;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && flag){
        if(is_custom_suffix[father[i]] && is_custom_suffix[child]){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child,S+father[i]);
            if(temp_res<0) flag=false;
            else i++;
        }
        //A questo punto solo uno o nessuno dei due è arificiale
        //Se il primo è artificiale allora si inverte la regola
        else if (is_custom_suffix[father[i]]){
            if(father[i] >= icfl_list[icfl_list_size-1] && child >= icfl_list[icfl_list_size-1]) flag=false;
            else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child)) i++;
            else{
                if(father[i] >= icfl_list[icfl_list_size-1]) flag=false;
                else if(child >= icfl_list[icfl_list_size-1]){
                    if(strcmp(S+child,S+father[i])<0) i++;
                    else flag=false;
                }
                else{
                    if(father[i] > child) i++;
                    else{
                        //Questo uguale all'originale, c'è la strcmp 
                        if(strcmp(S+child,S+father[i])<0) flag=false;
                        else i++;
                    }

                }
            }
        }
        //Se nessuno dei due è custom:
        else if(father[i] >= icfl_list[icfl_list_size-1] && child >= icfl_list[icfl_list_size-1]) i++;
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child)) flag=false;
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]) i++;
            else if(child >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child,S+father[i])<0) flag=false;
                else i++;
            }
            else{
                if(father[i] > child) flag=false;
                else{
                    if(strcmp(S+child,S+father[i])<0) flag=false;
                    else i++;
                }

            }
        }

    }

    if(!flag) father.insert(father.begin()+i,child);
    else father.push_back(child);
}