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
    res.reserve(x.size()+y.size());

    int i,j,z;
    i=j=z=0;
    

    while(z != common_elements.size()){
        while(x[i] != common_elements[z]) res.push_back(x[i++]);
        i++;
        while(y[j] != common_elements[z]) res.push_back(y[j++]);
        j++;
        res.push_back(common_elements[z++]);
    }

    //while(i<x.size()) res.push_back(x[i++]);
    //while(j<y.size()) res.push_back(y[j++]);
    res.insert(res.end(),x.begin()+i,x.end());
    res.insert(res.end(),y.begin()+j,y.end());

    //printVec(res);
    return res;
}



std::vector<int> get_common_prefix_merge_4_multihreading_3(custom_prefix_trie* root,std::vector<int>& common){
    //if(root->node){
    //cout<<"Nodo :";
    //print_substring(root->node->suffix,root->node->suffix_len);
    //cout<<"\n";}

    if(root->sons.empty())
        return root->node->common_chain_of_suffiexes;

    std::vector<vector<int>> vectors_to_join;
    std::map<char,custom_prefix_trie*>::iterator it;
    std::vector<int> res,comp=common;

    if(root->node) comp=root->node->common_chain_of_suffiexes;
        
    for(it = root->sons.begin(); it != root->sons.end();++it)
        vectors_to_join.push_back(get_common_prefix_merge_4_multihreading_3(it->second,comp));
    res=vectors_to_join.at(0); 
    for(int i=1;i<vectors_to_join.size();++i)
        res=common_prefix_merge_4_multithreading_3(res,vectors_to_join.at(i),comp);

    //printVec(res);
    //delete_custom_prefix_trie(root);
    //printVec(res);
    return res;

}


std::vector<int> get_common_prefix_partition(suffix_tree_node* root){
    //cout<<"Node: \n";
    //print_substring(root->suffix,root->suffix_len);
    //cout<<"\n";
    std::vector<int> result,temp,common=root->common_chain_of_suffiexes;
    int position=0;
    //cout<<"common: \n";
    //printVec(common);
    if(root->sons.empty()){
        //cout<<"result: \n";
        //printVec(common);
        return common;
    }
    for(int i=0;i<root->sons.size();i++){
        temp=get_common_prefix_partition(root->sons[i]);
        if(root->sons[i]->min_father==-1){
            result.insert(result.end(),common.begin()+position,common.end());
            result.insert(result.end(),temp.begin(),temp.end());
            position=common.size();
        }
        else{
            result.insert(result.end(),common.begin()+position,common.begin()+root->sons[i]->min_father);
            result.insert(result.end(),temp.begin(),temp.end());
            if(root->sons[i]->max_father==-1) position=root->sons[i]->min_father;
            else position=root->sons[i]->max_father;
        }
    }
    result.insert(result.end(),common.begin()+position,common.end());
    //cout<<"result: \n";
    //printVec(result);
    return result;
}

std::vector<int> get_common_prefix_index_to_nodes(const char* S,suffix_tree_node* root){
    //cout<<"Node: \n";
    //print_substring(root->suffix,root->suffix_len);
    //cout<<"\n";
    std::vector<int> result,temp,rank=root->array_of_indexes;
    int position=0;
    //cout<<"common: \n";
    //printVec(common);
    std::map<int,std::vector<int>>::iterator it;
    for (it = root->index_to_nodes.begin(); it != root->index_to_nodes.end() && it->first<rank.size(); it++){
        result.insert(result.end(),rank.begin()+position,rank.begin()+it->first);
        root->index_to_nodes[it->first].push_back(rank[it->first]);
        quicksort_of_indexes_5_2(S,root->index_to_nodes[it->first],0);
        result.insert(result.end(),root->index_to_nodes[it->first].begin(),root->index_to_nodes[it->first].end());
        position=it->first+1;
    }
    result.insert(result.end(),rank.begin()+position,rank.end());
    quicksort_of_indexes_5_2(S,root->index_to_nodes[rank.size()],0);
    result.insert(result.end(),root->index_to_nodes[rank.size()].begin(),root->index_to_nodes[rank.size()].end());
    //cout<<"result: \n";
    //printVec(result);
    return result;
}