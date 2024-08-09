#include "custom_prefix_trie.h"

std::unordered_map<suffix_tree_node*,bool> nodes_map;
std::vector<suffix_tree_node*> nodes_list;
std::mutex mut_nodes_map;

custom_prefix_trie* init_custom_prefix_trie(){
    custom_prefix_trie* x = new(malloc(sizeof(custom_prefix_trie))) custom_prefix_trie{};
    //cout<<"Dict vuoto: "<<x.sons.empty()<<"\n";
    //cout<<"Dict size: "<<x.sons.size()<<"\n";
    x->node=build_suffix_tree_node(NULL,"\0",0);
    x->father=NULL;
    return x;
}

void
delete_custom_prefix_trie(custom_prefix_trie* x){
    delete_suffix_tree_node(x->node);
    cout<<"c\n";
    x->sons.clear();
    free(x);
}

void stampa_prefix_trie(custom_prefix_trie* root){

    std::map<char,custom_prefix_trie*>::iterator it;

    if (!root->sons.size() && root->node){
        cout<<"(";
        print_substring(root->node->suffix,root->node->suffix_len);
        cout<<"[";
        for(size_t j = 0; j<root->node->array_of_indexes.size();j++) cout<<root->node->array_of_indexes[j]<<",";
        cout<<"]";
        cout<<"[";
        for(size_t j = 0; j<root->node->custom_array_of_indexes.size();j++) cout<<root->node->custom_array_of_indexes[j]<<",";
        cout<<"])";
        return;
    }

    if(root->node){
        print_substring(root->node->suffix,root->node->suffix_len);
        cout<<"(";
        for(it = root->sons.begin(); it != root->sons.end();++it) stampa_prefix_trie(it->second);
        cout<<"[";
        for(size_t j = 0; j<root->node->array_of_indexes.size();j++)cout<<root->node->array_of_indexes[j]<<",";
        cout<<"]";
        cout<<"[";
        for(size_t j = 0; j<root->node->custom_array_of_indexes.size();j++)cout<<root->node->custom_array_of_indexes[j]<<",";
        cout<<"])";
    }

    else for(it = root->sons.begin(); it != root->sons.end();++it) stampa_prefix_trie(it->second);
    return;
}

custom_prefix_trie* creazione_albero_custom_prefix_trie(vector<int>& icfl_list,vector<int>& custom_icfl_list,const char* S,int lenght_of_word,int max_size,int custom_max_size,int n_threads){
    custom_prefix_trie* root = init_custom_prefix_trie();

    clock_t tStart;
    double itime;
    //omp_set_num_threads(std::thread::hardware_concurrency());
    omp_set_num_threads(n_threads);

    itime = omp_get_wtime();
    //std::vector<int> is_custom_vec = get_is_custom_vec(icfl_list,lenght_of_word);
    std::vector<int> is_custom_vec = get_is_custom_vec_random(icfl_list,custom_icfl_list,lenght_of_word);
    std::vector<int> factor_list = get_factor_list(icfl_list,lenght_of_word);
    std::vector<suffix_tree_node*> indice_nodo;
    indice_nodo.resize(lenght_of_word);
    nodes_list.reserve(lenght_of_word);
    //la chiave è sempre l'intero più piccolo della coppia, se il bool è false allora va inserito il più grande, il più piccolo altrimenti.
    std::unordered_map<int,std::unordered_map<int,bool>*> ord;
    printf("tot strutture d'appoggio Time taken: %.2fs\n", omp_get_wtime() - itime);

    //cout<<"creato root\n";
    //for(int i=0;i<max_size;++i)
    //    compute_i_phase_alberello_custom_prefix_trie(S,lenght_of_word,icfl_list,icfl_list.size(),&root,i);   

    itime = omp_get_wtime();
    //#pragma omp parallel for //shared(S,lenght_of_word,icfl_list,custom_icfl_list,roots,mutex_m) schedule(static) 
    for(int i=0;i<custom_max_size;++i)
        compute_i_phase_alberello_custom_prefix_trie(S,lenght_of_word,icfl_list,icfl_list.size(),custom_icfl_list,custom_icfl_list.size(),root,i,is_custom_vec,factor_list,indice_nodo);
    printf("tot compute_i_phase_alberello_custom_prefix_trie Time taken: %.2fs\n", omp_get_wtime() - itime);

    //stampa_prefix_trie(root);
    //cout<<"\n";

    //itime = omp_get_wtime();
    //#pragma omp parallel for
    //for(int i=0;i<lenght_of_word;++i) 
    //    compute_ord(ord,S,indice_nodo,is_custom_vec,i,lenght_of_word);
    //printf("tot compute_ord Time taken: %.2fs\n", omp_get_wtime() - itime);
//
    ////stampa_ord(ord);
    ////for(int i=0;i<indice_nodo.size();++i) cout<<"i: "<<i<<", ref: "<<indice_nodo.at(i)<<"\n";
//
    //itime = omp_get_wtime();
    //merge_custom_array_of_indexes_prefix_trie_recurive(S,icfl_list,root,is_custom_vec,factor_list,ord);
    //printf("tot merge_custom_array_of_indexes_prefix_trie_recurive Time taken: %.2fs\n", omp_get_wtime() - itime);

    int node_list_size=nodes_list.size();
    cout<<"nodes_list.size(): "<<node_list_size<<"\n";
    //Merge di ogni singolo nodo
    itime = omp_get_wtime();
    #pragma omp parallel for shared(S,nodes_list,icfl_list,is_custom_vec,factor_list,ord)
    for(int i=0;i<node_list_size;++i)
        merge_single_node_2(S,nodes_list.at(i),icfl_list,is_custom_vec,factor_list,ord);
        //merge_single_node_3(S,nodes_list.at(i),icfl_list,is_custom_vec,factor_list,ord,lenght_of_word);
    printf("tot merge_single_node_2 Time taken: %.2fs\n", omp_get_wtime() - itime);


    //stampa_prefix_trie(root);
    //cout<<"\n";

    //itime = omp_get_wtime();
    //get_chain_from_root_2(S,icfl_list,icfl_list.size(),root,root->node->array_of_indexes,is_custom_vec,factor_list);
    //printf("tot get_chain_from_root_2 Time taken: %.2fs\n", omp_get_wtime() - itime);
    
    //std::vector<custom_prefix_trie*> sons_of_root;
    //for(std::map<char,custom_prefix_trie*>::iterator it = root->sons.begin(); it != root->sons.end();++it)
    //    sons_of_root.push_back(it->second);
//
    //itime = omp_get_wtime();
    //#pragma omp parallel for
    //for(int i=0;i<sons_of_root.size();++i)
    //    get_chain_from_root_2(S,icfl_list,icfl_list.size(),sons_of_root.at(i),root->node->array_of_indexes,is_custom_vec,factor_list);
    //printf("tot get_chain_from_root_2 Time taken: %.2fs\n", omp_get_wtime() - itime);


    itime = omp_get_wtime();
    //#pragma omp parallel for
    for(int i=0;i<node_list_size;++i)
        in_prefix_merge_bit_vector_5_5(S,icfl_list,icfl_list.size(),nodes_list.at(i)->father->common_chain_of_suffiexes,nodes_list.at(i)->array_of_indexes,nodes_list.at(i)->common_chain_of_suffiexes,is_custom_vec,factor_list);
    printf("tot in_prefix_merge_bit_vector_5_5 Time taken: %.2fs\n", omp_get_wtime() - itime);


    //stampa_prefix_trie(root);
    //cout<<"\n";

    return root;
}

void add_in_custom_prefix_trie(custom_prefix_trie* root,const char* S,const char* suffix,int current_suffix_len,int suffix_len,int suffix_index,vector<int>& icfl_list,vector<int>& custom_icfl_list,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo){
    //cout<<"Carattere: "<<suffix[current_suffix_len]<<", current_suffix_len: "<<current_suffix_len<<", suffix_len: "<<suffix_len<<"\n";
    custom_prefix_trie* opt_node=root;
    suffix_tree_node* father=NULL;
    while(current_suffix_len!=suffix_len){
        
        if(opt_node->sons.find(suffix[current_suffix_len]) == opt_node->sons.end())
            opt_node->sons[suffix[current_suffix_len]] = new(malloc(sizeof(custom_prefix_trie))) custom_prefix_trie{};

        //controllo se è un possibile padre
        if(current_suffix_len<suffix_len && opt_node->node) father=opt_node->node;

        opt_node=opt_node->sons[suffix[current_suffix_len++]];
    }

    if(!opt_node->node)
        //Il padre è null tanto verrà gestito con custom_prefix_trie
        //Anche i figli
        opt_node->node = build_suffix_tree_node_2(NULL,suffix,suffix_len);

    //aggiungo il padre
    opt_node->node->father=father;

    if(is_custom_vec[suffix_index]) opt_node->node->custom_array_of_indexes.push_back(suffix_index);
    else opt_node->node->array_of_indexes.push_back(suffix_index);

    indice_nodo.at(suffix_index) = opt_node->node;

    //mut_nodes_map.lock();
    if(nodes_map.find(opt_node->node) == nodes_map.end()){
        nodes_list.push_back(opt_node->node);
        nodes_map[opt_node->node]=true;
    }
    //mut_nodes_map.unlock();
    return;
}


void compute_i_phase_alberello_custom_prefix_trie(const char*S,int lenght_of_word,vector<int>& icfl_list,int icfl_size,vector<int>& custom_icfl_list,int custom_icfl_size,custom_prefix_trie* root,int i,vector<int>& is_custom_vec,vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo){
    //print_nodes_vector(alb->roots);
    if(i< lenght_of_word - custom_icfl_list[custom_icfl_size-1])
        //add_suffix_in_node_sons(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
        add_in_custom_prefix_trie(root,S,S  + lenght_of_word -1-i,0,i+1,lenght_of_word-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list,indice_nodo);
    //print_nodes_vector(alb->roots);
    for(int j=0;j<custom_icfl_size-1;++j)
        if(i<custom_icfl_list[j+1]-custom_icfl_list[j])
            add_in_custom_prefix_trie(root,S,S + custom_icfl_list[j] +custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,0,i+1,custom_icfl_list[j]+custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list,indice_nodo);
    //print_nodes_vector(alb->roots);
}

void merge_custom_array_of_indexes_prefix_trie_recurive(const char* S,vector<int>& icfl_list,custom_prefix_trie* root,std::vector<int> &is_custom_suffix, std::vector<int> &factor_list,std::unordered_map<int,std::unordered_map<int,bool>*> ord){
    std::vector<std::thread> threads;
    std::map<char,custom_prefix_trie*>::iterator it;
    //#pragma omp parallel for
    for(it = (*root).sons.begin(); it != (*root).sons.end(); ++it){
        if(it->second->node){
            //merge_single_node(S,it->second,icfl_list,is_custom_suffix,factor_list);
            threads.emplace_back(merge_single_node,S,*(it->second),std::ref(icfl_list),std::ref(is_custom_suffix),std::ref(factor_list),std::ref(ord));
        }
    }
    //cout<<"aaa\n";

    for (std::thread & th : threads){if (th.joinable())th.join();}

    for(it = (*root).sons.begin(); it != (*root).sons.end(); ++it)
        merge_custom_array_of_indexes_prefix_trie_recurive(S,icfl_list,it->second,is_custom_suffix,factor_list,ord);
}


//per prefix_trie
void get_chain_from_root_2(const char* S,std::vector<int>& icfl_list,const int& icfl_list_size,custom_prefix_trie * root,std::vector<int>& father_vector,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::map<char,custom_prefix_trie*>::iterator it;
    if(root->node){
        //root->node->common_chain_of_suffiexes = in_prefix_merge_bit_vector_5_3(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,is_custom_suffix,factor_list);
        
        //canonico
        //in_prefix_merge_bit_vector_5_4(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        //canonico con ricerca binaria
        in_prefix_merge_bit_vector_5_5(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        //sperimentale
        //in_prefix_merge_bit_vector_5_6(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        //if(root->node->array_of_indexes.size()<1000) alternative_prefix_merge_bit_vector(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        //else in_prefix_merge_bit_vector_5_4(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        
        //usa qsort, non migliore
        //alternative_prefix_merge_bit_vector_2(S,icfl_list,icfl_list_size,father_vector,root->node->array_of_indexes,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
    
        for(it = root->sons.begin(); it != root->sons.end();++it)
            get_chain_from_root_2(S,icfl_list,icfl_list_size,it->second,root->node->common_chain_of_suffiexes,is_custom_suffix,factor_list);
        return;
    }
    for(it = root->sons.begin(); it != root->sons.end();++it)
        get_chain_from_root_2(S,icfl_list,icfl_list_size,it->second,father_vector,is_custom_suffix,factor_list);
        
}

void merge_single_node(const char* S,custom_prefix_trie trie_node,std::vector<int> &icfl_list, std::vector<int> &is_custom_suffix, std::vector<int> &factor_list,std::unordered_map<int,std::unordered_map<int,bool>*>& ord){
    quicksort_of_indexes_2(S,trie_node.node->custom_array_of_indexes,0,trie_node.node->custom_array_of_indexes.size()-1,trie_node.node->suffix_len);
    //quicksort_of_indexes_3(S,trie_node.node->custom_array_of_indexes,0,trie_node.node->custom_array_of_indexes.size()-1,trie_node.node->suffix_len,ord);
    trie_node.node->array_of_indexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list.size(),trie_node.node->array_of_indexes,trie_node.node->custom_array_of_indexes,is_custom_suffix,trie_node.node->suffix_len,factor_list);
}

void merge_single_node_2(const char* S,suffix_tree_node* node,std::vector<int> &icfl_list, std::vector<int> &is_custom_suffix, std::vector<int> &factor_list,std::unordered_map<int,std::unordered_map<int,bool>*>& ord){
    //cout<<"Number of indexes: "<<node->custom_array_of_indexes.size()<<"\n";
    //quicksort_of_indexes_2(S,node->custom_array_of_indexes,0,node->custom_array_of_indexes.size()-1,node->suffix_len);
    //quicksort_of_indexes_3(S,node->custom_array_of_indexes,0,node->custom_array_of_indexes.size()-1,node->suffix_len,ord);
    //altrettando bello, usa std::sort par
    //quicksort_of_indexes_4(S,node->custom_array_of_indexes);
    //quicksort_of_indexes_4_2(S,node->custom_array_of_indexes,node->suffix_len);
    //molto bellissimo
    //quicksort_of_indexes_5(S,node->custom_array_of_indexes); 
    quicksort_of_indexes_5_2(S,node->custom_array_of_indexes,node->suffix_len);
    //quicksort_of_indexes_8(S,node->custom_array_of_indexes);
    node->array_of_indexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list.size(),node->array_of_indexes,node->custom_array_of_indexes,is_custom_suffix,node->suffix_len,factor_list);
}

void merge_single_node_3(const char* S,suffix_tree_node* node,std::vector<int> &icfl_list, std::vector<int> &is_custom_suffix, std::vector<int> &factor_list,std::unordered_map<int,std::unordered_map<int,bool>*>& ord,int lenght_of_word){
    quicksort_of_indexes_7(S,node->custom_array_of_indexes,lenght_of_word);
    node->array_of_indexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list.size(),node->array_of_indexes,node->custom_array_of_indexes,is_custom_suffix,node->suffix_len,factor_list);
}

//0 se il primo, 1 se il secondo
int who_comes_first(suffix_tree_node* root,int a,int b){
    for(int i=0;i<root->array_of_indexes.size();++i){
        if(root->array_of_indexes.at(i)==a) return 0;
        else if(root->array_of_indexes.at(i)==b) return 1;
    }
    return -1;
}

void compute_ord(std::unordered_map<int,std::unordered_map<int,bool>*>& ord,const char* S, std::vector<suffix_tree_node*>& indice_nodo,std::vector<int>& is_custom_vec,int distanza,int lenght_of_word){
    //rule = true allora viene inserito prima quello più grande, inverso altrimenti
    ord[distanza]=new std::unordered_map<int,bool>();
    bool rule=true;
    bool rush=false;
    for(int i=0;(i+distanza)<lenght_of_word;++i){
        if(indice_nodo.at(i)==indice_nodo.at(i+distanza)){
            if(!rush){
                if(!is_custom_vec.at(i)&&!is_custom_vec.at(i+distanza)){
                    if(!who_comes_first(indice_nodo.at(i),i,i+distanza)) rule=false;
                    else rule=true;
                }
                else{
                    //viene inserito prima l'indice più grande
                    if(strcmp(S+i+indice_nodo.at(i)->suffix_len,S+i+distanza+indice_nodo.at(i)->suffix_len)>0) rule = true;
                    else rule = false;
                }
                rush=true;
            }
            if(is_custom_vec.at(i)&&is_custom_vec.at(i+distanza)) (*ord[distanza])[i] = rule;
        }
        else rush=false;
    }
}

void stampa_ord(std::unordered_map<int,std::unordered_map<int,bool>*>& ord){
    for(std::unordered_map<int,std::unordered_map<int,bool>*>::iterator it = ord.begin(); it != ord.end();++it){
        cout<<"Distanza: "<<it->first<<"\n";
        std::unordered_map<int,bool> temp_map = (*it->second);
        for(std::unordered_map<int,bool>::iterator it2 = temp_map.begin(); it2 != temp_map.end();++it2){
            cout<<"primo: "<<it2->first<<", second: "<<it2->second+it->first<<", viene prima: "<<it2->second<<"\n";
        }
    }
}