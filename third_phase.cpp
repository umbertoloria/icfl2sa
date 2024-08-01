#include "third_phase.h"

using namespace std;

std::vector<int> common_prefix_merge_4_multithreading_2(std::vector<int> x,std::vector<int> y, std::vector<int> common_elements){
    //print_int_vector(x);
    //print_int_vector(y);
    //print_int_vector(common_elements);
    //cout<<"b\n";

    std::vector<int> res;

    int i,j,z;
    i=j=z=0;
    

    while(z != common_elements.size()){
        while(x[i] != common_elements[z]) res.push_back(x[i++]);
        i++;
        while(y[j] != common_elements[z]) res.push_back(y[j++]);
        j++;
        res.push_back(common_elements[z++]);
    }

    while(i<x.size()) res.push_back(x[i++]);
    while(j<y.size()) res.push_back(y[j++]);

    return res;
}



vector<int> get_common_prefix_merge_4_multihreading_2(suffix_tree_node* root){
    //cout<<"Nodo :";
    //print_substring(root->suffix,root->suffix_len);
    //cout<<"\n";
    vector<int> res;

    if(root->sons.empty()){
        res = get_chain_from_bit_vector_3(root);
        //cout<<"\nOttenuta chain\n";
        //printVec(res);
        return res;
    }  
    int k=root->sons.size(),next_k;

    //cout<<"ciao\n";

    vector<int> common_elements=get_chain_from_bit_vector_3(root);
    vector<int> common_prefix_of_sons[k];

    //cout<<"ciao\n";

    for(int z=0;z<k;z++)
        common_prefix_of_sons[z]=get_common_prefix_merge_4_multihreading_2(root->sons[z]);

    while(k>1){
        if (k%2==1) next_k=k/2+1;
        else next_k=k/2;

        for(int j=0;j<k/2;j++)
            common_prefix_of_sons[j]=common_prefix_merge_4_multithreading_2(common_prefix_of_sons[j*2],common_prefix_of_sons[(j*2)+1],common_elements);
        
        if(k%2==1) common_prefix_of_sons[next_k-1]=common_prefix_of_sons[k-1];

        //Da fare alla fine
        k=next_k;
    }

    res=common_prefix_of_sons[0];
    //printVec(res);
    return res;

}

//PREFIX TRIE

std::vector<int> common_prefix_merge_4_multithreading_3(std::vector<int>& x,std::vector<int>& y, std::vector<int>& common_elements){
    //printVec(x);
    //printVec(y);
    //printVec(common_elements);
    //cout<<"b\n";

    std::vector<int> res;

    int i,j,z;
    i=j=z=0;
    

    while(z != common_elements.size()){
        while(x[i] != common_elements[z]) res.push_back(x[i++]);
        i++;
        while(y[j] != common_elements[z]) res.push_back(y[j++]);
        j++;
        res.push_back(common_elements[z++]);
    }

    while(i<x.size()) res.push_back(x[i++]);
    while(j<y.size()) res.push_back(y[j++]);

    //printVec(res);
    return res;
}



std::vector<int> get_common_prefix_merge_4_multihreading_3(custom_prefix_trie* root,std::vector<int>& common){
    //if(root->node){
    //cout<<"Nodo :";
    //print_substring(root->node->suffix,root->node->suffix_len);
    //cout<<"\n";}

    //flag per capire se è un nodo vuoto
    bool flag=true;

    if(root->node) flag=false;

    if(root->sons.empty())
        return root->node->common_chain_of_suffiexes;

    std::vector<vector<int>> vectors_to_join;
    std::map<char,custom_prefix_trie*>::iterator it;

    for(it = root->sons.begin(); it != root->sons.end();++it){
       if(flag) vectors_to_join.push_back(get_common_prefix_merge_4_multihreading_3(it->second,common));
       else vectors_to_join.push_back(get_common_prefix_merge_4_multihreading_3(it->second,root->node->common_chain_of_suffiexes));
    }
    std::vector<int> res=vectors_to_join.at(0);

    for(int i=1;i<vectors_to_join.size();++i){
        if(flag) res=common_prefix_merge_4_multithreading_3(res,vectors_to_join.at(i),common);
        else res=common_prefix_merge_4_multithreading_3(res,vectors_to_join.at(i),root->node->common_chain_of_suffiexes);
    }



    return res;

}