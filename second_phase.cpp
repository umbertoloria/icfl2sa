
#include "second_phase.h"


using namespace std;

vector<int> join_int_vector_with_bit_vector(vector<int> father_chain,vector<int>& son_chain,vector<bool>& bit_vec){
    //i=indice array dei suffissi del nodo padre
    //j=indice array dei suffissi del nodo figlio
    //z=indice bit vector del nodo figlio
    int i,j;
    i=j=0;
    vector<int> result;
    result.reserve(son_chain.size()+father_chain.size());
    //#pragma omp parallel for shared(result)
    for(int z=0;z<bit_vec.size();++z){
        if(bit_vec[z]) result.emplace_back(father_chain[i++]);
        else result.emplace_back(son_chain[j++]);
        //if(bit_vec[z]) result[z]=father_chain[i++];
        //else result[z]=son_chain[j++];
    }
    return result;
}

vector<int> join_int_vector_with_bit_vector_2(vector<int> father_chain,vector<int>& son_chain,vector<bool>& bit_vec){
    //i=indice array dei suffissi del nodo padre
    //j=indice array dei suffissi del nodo figlio
    //z=indice bit vector del nodo figlio
    int i,j;
    i=j=0;
    vector<int> result;
    result.resize(bit_vec.size());
    //#pragma omp parallel for shared(result)
    for(int z=0;z<bit_vec.size();++z){
        if(bit_vec[z]) result.at(z)=father_chain[i++];
        else result.at(z)=son_chain[j++];
        //if(bit_vec[z]) result[z]=father_chain[i++];
        //else result[z]=son_chain[j++];
    }
    return result;
}

//O usiamo qusta ovunque, oppure no
//Fa affidamento sul fatto che gli array vengono duplicati quindi non si possono fare determinate assunzioni
//Tipo fare attenzione a quando si usa la free()
vector<int> get_chain_from_bit_vector_3(suffix_tree_node* root){
    if(root->father==NULL) return root->array_of_indexes;
    if(!root->common_chain_of_suffiexes.empty()) return root->common_chain_of_suffiexes;
    return join_int_vector_with_bit_vector_2(get_chain_from_bit_vector_3(root->father),root->array_of_indexes,root->bit_vec);
}

//vecchia implementazione senza std::vector<int>& is_custom_suffix
void create_bit_vector_3(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root){
    vector<int> father_chain = get_chain_from_bit_vector_3(root->father);
    //root->bit_vec=in_prefix_merge_bit_vector_5(S,icfl_list,icfl_list_size,father_chain,root->array_of_indexes,root->father->suffix_len);
}


void create_bit_vector_3_redundancy(const char* S,vector<int>& icfl_list,int icfl_list_size, suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    root->bit_vec=in_prefix_merge_bit_vector_5_1(S,icfl_list,icfl_list_size,get_chain_from_bit_vector_3(root->father),root->array_of_indexes,root->father->suffix_len,is_custom_suffix,factor_list);
    root->common_chain_of_suffiexes = get_chain_from_bit_vector_3(root);
}

void create_chain(const char* S,vector<int>& icfl_list,int icfl_list_size, suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    if(root->father==NULL)
        root->common_chain_of_suffiexes = in_prefix_merge_bit_vector_5_2(S,icfl_list,icfl_list_size,root->father->array_of_indexes,root->array_of_indexes,root->father->suffix_len,is_custom_suffix,factor_list);
        //root->common_chain_of_suffiexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list_size,root->father->array_of_indexes,root->array_of_indexes,is_custom_suffix,root->father->suffix_len,factor_list);
    else
        root->common_chain_of_suffiexes = in_prefix_merge_bit_vector_5_2(S,icfl_list,icfl_list_size,root->father->common_chain_of_suffiexes,root->array_of_indexes,root->father->suffix_len,is_custom_suffix,factor_list);
        //root->common_chain_of_suffiexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list_size,root->father->common_chain_of_suffiexes,root->array_of_indexes,is_custom_suffix,root->father->suffix_len,factor_list);
}

//Utilizza i common elements
//void create_bit_vector_4(const char* S,vector<int> icfl_list,int icfl_list_size, suffix_tree_node* root){
//
//    int_vector* father_chain = get_chain_from_common_elements_vector(root->father);
//    //cout<<"\nottenuta catena del padre\n";
//    root->common_elements_vec=in_prefix_merge_bit_vector_5(S,icfl_list,icfl_list_size,father_chain,root->array_of_indexes);
//
//}


void get_bit_vectors_from_root(const char* S,vector<int>& icfl_list,int icfl_list_size,suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    create_bit_vector_3_redundancy(S,icfl_list,icfl_list_size,root,is_custom_suffix,factor_list);
    for(int i=0;i<root->sons.size();i++) 
        get_bit_vectors_from_root(S,icfl_list,icfl_list_size,root->sons[i],is_custom_suffix,factor_list);
    return;
}

void get_chain_from_root(const char* S,vector<int>& icfl_list,int icfl_list_size,suffix_tree_node* root,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    create_chain(S,icfl_list,icfl_list_size,root,is_custom_suffix,factor_list);
    for(int i=0;i<root->sons.size();i++) 
        get_chain_from_root(S,icfl_list,icfl_list_size,root->sons[i],is_custom_suffix,factor_list);
    return;
}

