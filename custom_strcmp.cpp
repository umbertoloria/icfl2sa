#include "custom_strcmp.h"

int custom_strcmp(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo) {
    int temp_res;
    while (*(S+x) && (*(S+x) == *(S+y))) {
        if((!is_custom_suffix[x] || !is_custom_suffix[y]) && (indice_nodo[x]==indice_nodo[y])){
            //cout<<"x: "<<x<<", y:"<<y<<"\n";
            temp_res=rules_custom_strcmp(S,icfl_list,icfl_list_size,x,y,child_offset,is_custom_suffix,factor_list,indice_nodo);
            if(temp_res!=0) return temp_res;
        }
        x++;
        y++;
    }
    return *(unsigned char *)(S+x) - *(unsigned char *)(S+y);
}

//return 1 significa che viene inseito prima y, -1 prima x e 0 ancora non si sa.
int rules_custom_strcmp(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo){
    
    if(is_custom_suffix[x] && is_custom_suffix[y] ) return 0;
    else if(is_custom_suffix[x]){
        if(factor_list[x]<=factor_list[y]){
            if(y >= icfl_list[icfl_list_size-1])return 1;
            else return -1;
        } 
        else return 0;
    }
    else if(is_custom_suffix[y]){
        if(factor_list[y]<=factor_list[x]){
            if(x >= icfl_list[icfl_list_size-1]) return -1;
            else return 1;
        }
        else return 0;
    }
    else if(x >= icfl_list[icfl_list_size-1] && y >= icfl_list[icfl_list_size-1]){return -1;}
    else if(factor_list[x]==factor_list[y]){return 1;}
    else{
        if(x >= icfl_list[icfl_list_size-1]){return -1;}
        else if(y >= icfl_list[icfl_list_size-1]) return 0;
        else{
            if(x > y){return 1;}
            else return 0;
        }
    }
}