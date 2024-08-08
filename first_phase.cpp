#include "first_phase.h"


std::mutex mut_map;

int get_max_size(vector<int> icfl_list,int lenght_of_word){
    int max_size=-1;
    for(int i=1;i<icfl_list.size();i++){
        if (icfl_list[i]-icfl_list[i-1] > max_size){
            max_size=icfl_list[i]-icfl_list[i-1];
        } 
    }
    if(lenght_of_word-icfl_list[icfl_list.size()-1] > max_size) max_size = lenght_of_word-icfl_list[icfl_list.size()-1];
    return max_size;
}

suffix_tree_node* creazione_albero_alberelli(vector<int>& icfl_list,vector<int>& custom_icfl_list,const char* S,int lenght_of_word,int max_size,int custom_max_size,int n_threads){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_size=icfl_list.size();
    //clock_t tot_inserimento=0,tot_bitvector=0,tStart;
    suffix_tree_node** roots=(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*max_size);
    //std::vector<suffix_tree_node*> roots[max_size];
    clock_t tStart;
    double itime;
    //omp_set_num_threads(std::thread::hardware_concurrency());
    omp_set_num_threads(n_threads);

    std::unordered_map<size_t,std::vector<suffix_tree_node*>> m;
    std::mutex mutex_m;

    std::vector<int> is_custom_vec = get_is_custom_vec(icfl_list,lenght_of_word);
    std::vector<int> factor_list = get_factor_list(icfl_list,lenght_of_word);

    //Inizializza i nodi padre vuoti, uno per ogni size ddei suffissi
    #pragma omp parallel for //shared(roots) schedule(static)
    for(int i=0;i<custom_max_size;i++)
        roots[i]=build_suffix_tree_node(NULL,"\0",0);

    itime = omp_get_wtime();

    //i è la lunghezza del suffisso, per ogni lunghezza di un suffisso si inserisce nell'apposito nodo vuoto
    //il nodo vuoti è roots[i] e contiene tutti i suffissi di lunghezza i
    #pragma omp parallel for //shared(S,lenght_of_word,icfl_list,custom_icfl_list,roots,mutex_m) schedule(static) 
    for(int i=0;i<custom_max_size;++i)
        //SERIALE
        //compute_i_phase_alberello_2(S,lenght_of_word,icfl_list,icfl_size,roots[i],i);
        compute_i_phase_alberello_2(S,lenght_of_word,icfl_list,icfl_list.size(),custom_icfl_list,custom_icfl_list.size(),roots[i],i,is_custom_vec,factor_list);
        //PARALLELA (non buona)
        //compute_i_phase_alberello_3(S,lenght_of_word,icfl_list,icfl_size,roots[i],i,m,mutex_m);

    printf("tot inizializzazione Time taken: %.2fs\n", omp_get_wtime() - itime);

    //cout<<m.size()<<"\n";
    //cout<<get_hash_of_subsring(get_substring("AA",1))<<"\n";
    //cout<<m[get_hash_of_subsring(get_substring("A",1))].size()<<"\n";
    //print_nodes_vector_2(m[get_hash_of_subsring(get_substring("A",1))]);
    //cout<<"\n";
//
    //for(std::map<size_t,std::vector<suffix_tree_node*>>::iterator it = m.begin(); it != m.end(); ++it) {
    //  std::cout << "Key: " << it->first << std::endl;
    //  //std::cout << "Value: " << it->second.empty() << std::endl;
    //  print_nodes_vector_2(it->second);
    //  cout<<"\n";
    //}


    itime = omp_get_wtime();

    //Aggiungere fase di unione tra array di suffissi locali e custom
    #pragma omp parallel for //shared(S,lenght_of_word,icfl_list,icfl_size,roots,mutex_m) schedule(static) 
    for(int i=0;i<custom_max_size;++i)
        merge_custom_array_of_indexes(S,icfl_list,roots[i],is_custom_vec,factor_list);
    printf("tot merge_custom_array_of_indexes Time taken: %.2fs\n", omp_get_wtime() - itime);

    //for(int i=0;i<custom_max_size;i++)
    //  for(int j=0;j<roots[0]->sons.size();j++)
    //      create_bit_vector_3_redundancy(S,icfl_list,icfl_list.size(),roots[0]->sons[j]);
        //roots[0]->sons[i]->common_chain_of_suffiexes=roots[0]->sons[i]->array_of_indexes;

    itime = omp_get_wtime();

    //join_n_alberelli(roots,max_size,&root);
    //join_n_alberelli_multithreading(roots,max_size,&root);
    //join_n_alberelli_multithreading_2(roots,max_size,&root);
    join_n_alberelli_omp(roots,custom_max_size,&root,S,icfl_list);
    //join_n_alberelli_omp_2(roots,max_size,&root,m,mutex_m);

    printf("tot join Time taken: %.2fs\n", omp_get_wtime() - itime);

    //cout<<"finito il join\n";
    stampa_suffix_tree(root);
    //cout<<"\n";

    itime = omp_get_wtime();
    #pragma omp parallel for
    for(int i = 0;i<root->sons.size();i++)
        get_chain_from_root(S,icfl_list,icfl_size,root->sons[i],is_custom_vec,factor_list);
    printf("get_chain_from_root Time taken: %.2fs\n", omp_get_wtime() - itime);
    //tot_bitvector+=clock()-tStart;


    //printf("tot_inserimento Time taken: %.2fs\n", (double)(tot_inserimento)/CLOCKS_PER_SEC);
    //printf("tot_bitvector Time taken: %.2fs\n\n", (double)(tot_bitvector)/CLOCKS_PER_SEC);

    return root;
}

//root è l'alberello vuoto che contiene tutti i suffissi di lunghezza i
void compute_i_phase_alberello_2(const char*S,int lenght_of_word,vector<int>& icfl_list,int icfl_size,vector<int>& custom_icfl_list,int custom_icfl_size,suffix_tree_node* root,int i,vector<int>& is_custom_vec,vector<int>& factor_list){
    //print_nodes_vector(alb->roots);
    //controlla i se non sfora la max lenght per questo fattore
    if(i< lenght_of_word - custom_icfl_list[custom_icfl_size-1])
        //add_suffix_in_node_sons(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
        add_suffix_in_node_sons_4(root,S,S + custom_icfl_list[custom_icfl_size-1] + lenght_of_word - custom_icfl_list[custom_icfl_size-1]-1-i,i+1,custom_icfl_list[custom_icfl_size-1]+lenght_of_word - custom_icfl_list[custom_icfl_size-1]-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list);
    //print_nodes_vector(alb->roots);
    for(int j=0;j<custom_icfl_size-1;j++)
        //add_node_in_suffix_tree_alberello_2(S,icfl_list,icfl_size,custom_icfl_list,custom_icfl_size,root,i,j,lenght_of_word,is_custom_vec);
        if(i<custom_icfl_list[j+1]-custom_icfl_list[j])
            add_suffix_in_node_sons_4(root,S,S + custom_icfl_list[j] +custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,i+1,custom_icfl_list[j]+custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list);
    //print_nodes_vector(alb->roots);
}

//root è l'alberello vuoto che contiene tutti i suffissi di lunghezza i
void add_node_in_suffix_tree_alberello_2(const char* S,vector<int>& icfl_list,int icfl_size,vector<int>& custom_icfl_list,int custom_icfl_size,suffix_tree_node* root,int i,int j,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list){
    //print_nodes_vector(alb->roots);
    //controlla se i non sfora la max lenght per questo fattore
    if(i<custom_icfl_list[j+1]-custom_icfl_list[j])
        //add_suffix_in_node_sons(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,i+1);
        add_suffix_in_node_sons_2(root,S,S + custom_icfl_list[j] +custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,i+1,custom_icfl_list[j]+custom_icfl_list[j+1]-custom_icfl_list[j]-1-i,icfl_list,custom_icfl_list,lenght_of_word,is_custom_vec,factor_list);
    //cout<<"aggiunto in alberello\n";
}


void compute_i_phase_alberello_3(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    suffix_tree_node* x;
    std::vector<suffix_tree_node*> y;
    if(i< lenght_of_word - icfl_list[icfl_size-1]){
        x=add_suffix_in_node_sons_3(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i);
        size_t key=hash_substring(x->suffix,x->suffix_len);
        //mut_map.lock();
        std::lock_guard<std::mutex> lock(mutex_m);
        add_node_in_node_sons_5_map(m[key],x);
        //mut_map.unlock();
    }
    for(int j=0;j<icfl_size-1;j++)
        add_node_in_suffix_tree_alberello_3(S,icfl_list,icfl_size,root,i,j,m,mutex_m);
}

void add_node_in_suffix_tree_alberello_3(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    suffix_tree_node* x;
    std::vector<suffix_tree_node*> y;
    if(i<icfl_list[j+1]-icfl_list[j]){
        x=add_suffix_in_node_sons_3(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,i+1,icfl_list[j]+icfl_list[j+1]-icfl_list[j]-1-i);
        size_t key=hash_substring(x->suffix,x->suffix_len);
        //mut_map.lock();
        std::lock_guard<std::mutex> lock(mutex_m);
        add_node_in_node_sons_5_map(m[key],x);
        //mut_map.unlock();
    }
}

vector<int> get_is_custom_vec(vector<int>&icfl ,int lenght_of_word){
    vector<int> is_custom_vec;
    is_custom_vec.resize(lenght_of_word);
    // 0 = fattore ICFL
    // 1 = fattore custom
    #pragma omp parallel for
    for(int i=0;i<lenght_of_word;++i)
        is_custom_vec.at(i)=check_if_custom_index(icfl,lenght_of_word,i);
    return is_custom_vec;
}

vector<int> get_is_custom_vec_random(vector<int>& icfl,vector<int>& custom_icfl,int lenght_of_word){
    vector<int> is_custom_vec;
    is_custom_vec.resize(lenght_of_word);
    // 0 = fattore ICFL
    // 1 = fattore custom
    #pragma omp parallel for
    for(int i=0;i<lenght_of_word;++i)
        is_custom_vec.at(i)=check_if_custom_index_random(icfl,custom_icfl,lenght_of_word,i);
    return is_custom_vec;
}
