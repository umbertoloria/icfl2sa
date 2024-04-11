#include "custom_prefix_trie.h"

custom_prefix_trie init_custom_prefix_trie(){
    custom_prefix_trie x;
    //cout<<"Dict vuoto: "<<x.sons.empty()<<"\n";
    //cout<<"Dict size: "<<x.sons.size()<<"\n";
    x.node=NULL;
    return x;
}

custom_prefix_trie creazione_albero_custom_prefix_trie(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    custom_prefix_trie root = init_custom_prefix_trie();
    //cout<<"creato root\n";
    for(int i=0;i<max_size;++i)
        compute_i_phase_alberello_custom_prefix_trie(S,lenght_of_word,icfl_list,icfl_list.size(),&root,i);
    return root;
}

void add_in_custom_prefix_trie(custom_prefix_trie* root,const char* suffix,int current_suffix_len,int suffix_len,int suffix_index){
    //cout<<"Carattere: "<<suffix[current_suffix_len]<<", current_suffix_len: "<<current_suffix_len<<", suffix_len: "<<suffix_len<<"\n";

    if(current_suffix_len==suffix_index){
        if(!root->node)
            root->node = build_suffix_tree_node(NULL,suffix,suffix_index);
        root->node->array_of_indexes.push_back(suffix_index);
        return;
    }
    
    std::map<char,custom_prefix_trie>::iterator iter;

    //cout<<suffix[current_suffix_len]<<"\n";
    
    if(!root->sons.empty())
        iter = root->sons.find(suffix[current_suffix_len]);
    
    if(root->sons.empty() || iter ==  root->sons.end()){
        //cout<<"non presente\n";
        custom_prefix_trie temp = init_custom_prefix_trie();
        root->sons.insert(std::pair<char,custom_prefix_trie>(suffix[current_suffix_len],temp));
        return add_in_custom_prefix_trie(&temp,suffix,++current_suffix_len,suffix_len,suffix_index);
    }
    //cout<<"presente\n";
    return add_in_custom_prefix_trie(&iter->second,suffix,++current_suffix_len,suffix_len,suffix_index);
}


void compute_i_phase_alberello_custom_prefix_trie(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,custom_prefix_trie * root,int i){
    //print_nodes_vector(alb->roots);
    if(i< lenght_of_word - icfl_list[icfl_size-1])
        //add_suffix_in_node_sons(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
        add_in_custom_prefix_trie(root,S  + lenght_of_word -1-i,0,i+1,lenght_of_word-1-i);
    //print_nodes_vector(alb->roots);
    for(int j=0;j<icfl_size-1;++j)
        add_node_in_suffix_tree_alberello_custom_prefix_trie(S,icfl_list,icfl_size,root,i,j);
    //print_nodes_vector(alb->roots);
}

void add_node_in_suffix_tree_alberello_custom_prefix_trie(const char* S,vector<int> icfl_list,int icfl_size,custom_prefix_trie * root,int i,int j){
    //print_nodes_vector(alb->roots);
    if(i<icfl_list[j+1]-icfl_list[j])
        add_in_custom_prefix_trie(root,S + icfl_list[j+1] -1-i,0,i+1,icfl_list[j+1]-1-i);
    //cout<<"aggiunto in alberello\n";
}
