#include "custom_prefix_trie.h"

custom_prefix_trie* init_custom_prefix_trie(){
    custom_prefix_trie* x = new(malloc(sizeof(custom_prefix_trie))) custom_prefix_trie{};
    //cout<<"Dict vuoto: "<<x.sons.empty()<<"\n";
    //cout<<"Dict size: "<<x.sons.size()<<"\n";
    x->node=build_suffix_tree_node(NULL,"\0",0);
    x->father=NULL;
    return x;
}

void stampa_prefix_trie(custom_prefix_trie* root){

    std::map<char,custom_prefix_trie>::iterator it;

    if (!root->sons.size() && root->node){
        cout<<"(";
        print_substring(root->node->suffix,root->node->suffix_len);
        cout<<"[";
        for(size_t j = 0; j<root->node->array_of_indexes.size();j++){
            cout<<root->node->array_of_indexes[j]<<",";
        }
        cout<<"])";
        return;
    }

    if(root->node){
        print_substring(root->node->suffix,root->node->suffix_len);
        cout<<"(";
        for(it = root->sons.begin(); it != root->sons.end();++it){
            stampa_prefix_trie(&it->second);
        }
        cout<<"[";
            for(size_t j = 0; j<root->node->array_of_indexes.size();j++){
                cout<<root->node->array_of_indexes[j]<<",";
            }
        cout<<"])";
    }

    else for(it = root->sons.begin(); it != root->sons.end();++it) stampa_prefix_trie(&it->second);
    return;
}

custom_prefix_trie* creazione_albero_custom_prefix_trie(vector<int>& icfl_list,vector<int>& custom_icfl_list,const char* S,int lenght_of_word,int max_size,int custom_max_size,int n_threads){
    custom_prefix_trie* root = init_custom_prefix_trie();

    clock_t tStart;
    double itime;
    //omp_set_num_threads(std::thread::hardware_concurrency());
    omp_set_num_threads(n_threads);

    std::vector<int> is_custom_vec = get_is_custom_vec(icfl_list,lenght_of_word);
    std::vector<int> factor_list = get_factor_list(icfl_list,lenght_of_word);



    //cout<<"creato root\n";
    //for(int i=0;i<max_size;++i)
    //    compute_i_phase_alberello_custom_prefix_trie(S,lenght_of_word,icfl_list,icfl_list.size(),&root,i);   

    itime = omp_get_wtime();
    #pragma omp parallel for //shared(S,lenght_of_word,icfl_list,custom_icfl_list,roots,mutex_m) schedule(static) 
    for(int i=0;i<custom_max_size;++i)
        compute_i_phase_alberello_custom_prefix_trie(S,lenght_of_word,icfl_list,icfl_list.size(),custom_icfl_list,custom_icfl_list.size(),root,i,is_custom_vec,factor_list);
    printf("tot compute_i_phase_alberello_custom_prefix_trie Time taken: %.2fs\n", omp_get_wtime() - itime);

    itime = omp_get_wtime();
    merge_custom_array_of_indexes_prefix_trie_recurive(S,icfl_list,root,is_custom_vec,factor_list);
    printf("tot merge_custom_array_of_indexes_prefix_trie_recurive Time taken: %.2fs\n", omp_get_wtime() - itime);

    //stampa_prefix_trie(root);
    //cout<<"\n";

    itime = omp_get_wtime();
    get_chain_from_root_2(S,icfl_list,icfl_list.size(),root,root->node->array_of_indexes,is_custom_vec,factor_list);
    printf("tot get_chain_from_root_2 Time taken: %.2fs\n", omp_get_wtime() - itime);

    //stampa_prefix_trie(root);
    //cout<<"\n";

    return root;
}

void add_in_custom_prefix_trie(custom_prefix_trie* root,const char* S,const char* suffix,int current_suffix_len,int suffix_len,int suffix_index,vector<int>& icfl_list,vector<int>& custom_icfl_list,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list){
    //cout<<"Carattere: "<<suffix[current_suffix_len]<<", current_suffix_len: "<<current_suffix_len<<", suffix_len: "<<suffix_len<<"\n";
    custom_prefix_trie* opt_node=root;
    while(current_suffix_len!=suffix_len)
        opt_node=&opt_node->sons[suffix[current_suffix_len++]];

    if(!opt_node->node)
        //Il padre è null tanto verrà gestito con custom_prefix_trie
        //Anche i figli
        opt_node->node = build_suffix_tree_node_2(NULL,suffix,suffix_len);

    if(is_custom_vec[suffix_index]) opt_node->node->custom_array_of_indexes.push_back(suffix_index);
    else opt_node->node->array_of_indexes.push_back(suffix_index);
    return;
}


void compute_i_phase_alberello_custom_prefix_trie(const char*S,int lenght_of_word,vector<int>& icfl_list,int icfl_size,vector<int>& custom_icfl_list,int custom_icfl_size,custom_prefix_trie* root,int i,vector<int>& is_custom_vec,vector<int>& factor_list){
    //print_nodes_vector(alb->roots);
    if(i< lenght_of_word - custom_icfl_list[custom_icfl_size-1])
        //add_suffix_in_node_sons(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
        add_in_custom_prefix_trie(root,S,S  + lenght_of_word -1-i,0,i+1,lenght_of_word-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list);
    //print_nodes_vector(alb->roots);
    for(int j=0;j<custom_icfl_size-1;++j)
        if(i<custom_icfl_list[j+1]-custom_icfl_list[j])
            add_in_custom_prefix_trie(root,S,S + custom_icfl_list[j] +custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,0,i+1,custom_icfl_list[j]+custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list);
    //print_nodes_vector(alb->roots);
}

void merge_custom_array_of_indexes_prefix_trie_recurive(const char* S,vector<int>& icfl_list,custom_prefix_trie* root,std::vector<int> &is_custom_suffix, std::vector<int> &factor_list){
    
    std::map<char,custom_prefix_trie>::iterator it;
    //#pragma omp parallel for
    for(it = (*root).sons.begin(); it != (*root).sons.end(); ++it){
        if(it->second.node){
            quicksort_of_indexes_2(S,it->second.node->custom_array_of_indexes,0,it->second.node->custom_array_of_indexes.size()-1,it->second.node->suffix_len);
            it->second.node->array_of_indexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list.size(),it->second.node->array_of_indexes,it->second.node->custom_array_of_indexes,is_custom_suffix,it->second.node->suffix_len,factor_list);
        }
    }
    
    for(it = (*root).sons.begin(); it != (*root).sons.end(); ++it)
    //for (const auto& kv : (*root).sons) {
        merge_custom_array_of_indexes_prefix_trie_recurive(S,icfl_list,&it->second,is_custom_suffix,factor_list);
}


//per prefix_trie
void get_chain_from_root_2(const char* S,vector<int>& icfl_list,int icfl_list_size,custom_prefix_trie * root,std::vector<int>& father_vector,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    bool flag=true;
    //std::vector<int> temp=father_vector;
    if(root->node){
        //root->node->common_chain_of_suffiexes = in_prefix_merge_bit_vector_5_3(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,is_custom_suffix,factor_list);
        in_prefix_merge_bit_vector_5_4(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        //temp=root->node->common_chain_of_suffiexes;
        flag=false;
        //printVec(temp);
    }
    
    for(std::map<char,custom_prefix_trie>::iterator it = root->sons.begin(); it != root->sons.end();++it){
        if(flag) get_chain_from_root_2(S,icfl_list,icfl_list_size,&it->second,father_vector,is_custom_suffix,factor_list);
        else get_chain_from_root_2(S,icfl_list,icfl_list_size,&it->second,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
    }
    return;
}