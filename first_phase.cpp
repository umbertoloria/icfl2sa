#include "first_phase.h"
#include "second_phase.h"
#include <omp.h>

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

suffix_tree_node* creazione_albero_alberelli(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_size=icfl_list.size();
    //clock_t tot_inserimento=0,tot_bitvector=0,tStart;
    suffix_tree_node** roots=(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*max_size);
    //std::vector<suffix_tree_node*> roots[max_size];
    clock_t tStart;
    double itime;
    //omp_set_num_threads(std::thread::hardware_concurrency());
    omp_set_num_threads(std::thread::hardware_concurrency());
    itime = omp_get_wtime();

    std::map<size_t,std::vector<suffix_tree_node*>> m;



    #pragma omp parallel for shared(roots) schedule(static)
    for(int i=0;i<max_size;i++)
        roots[i]=build_suffix_tree_node(NULL,"\0",0);

    #pragma omp parallel for shared(S,lenght_of_word,icfl_list,icfl_size,roots) schedule(static) 
    for(int i=0;i<max_size;++i)
        compute_i_phase_alberello_2(S,lenght_of_word,icfl_list,icfl_size,roots[i],i);
        //compute_i_phase_alberello_3(S,lenght_of_word,icfl_list,icfl_size,roots[i],i,m);

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

    //join_n_alberelli(roots,max_size,&root);
    //join_n_alberelli_multithreading(roots,max_size,&root);
    //join_n_alberelli_multithreading_2(roots,max_size,&root);
    join_n_alberelli_omp(roots,max_size,&root);
    //join_n_alberelli_omp_2(roots,max_size,&root,m);

    printf("tot join Time taken: %.2fs\n", omp_get_wtime() - itime);

    //cout<<"finito il join\n";
    //stampa_suffix_tree(root);

    itime = omp_get_wtime();
    for(int i = 0;i<root->sons.size();i++)
        get_bit_vectors_from_root(S,icfl_list,icfl_size,root->sons[i]);
    printf("get_bit_vectors_from_root Time taken: %.2fs\n", omp_get_wtime() - itime);
    //tot_bitvector+=clock()-tStart;


    //printf("tot_inserimento Time taken: %.2fs\n", (double)(tot_inserimento)/CLOCKS_PER_SEC);
    //printf("tot_bitvector Time taken: %.2fs\n\n", (double)(tot_bitvector)/CLOCKS_PER_SEC);

    return root;
}


void compute_i_phase_alberello_2(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i){
    //print_nodes_vector(alb->roots);
    if(i< lenght_of_word - icfl_list[icfl_size-1])
        //add_suffix_in_node_sons(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
        add_suffix_in_node_sons_2(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i);
    //print_nodes_vector(alb->roots);
    for(int j=0;j<icfl_size-1;j++)
        add_node_in_suffix_tree_alberello_2(S,icfl_list,icfl_size,root,i,j);
    //print_nodes_vector(alb->roots);
}

void add_node_in_suffix_tree_alberello_2(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j){
    //print_nodes_vector(alb->roots);
    if(i<icfl_list[j+1]-icfl_list[j])
        //add_suffix_in_node_sons(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,i+1);
        add_suffix_in_node_sons_2(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,i+1,icfl_list[j]+icfl_list[j+1]-icfl_list[j]-1-i);
    //cout<<"aggiunto in alberello\n";
}


void compute_i_phase_alberello_3(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i,std::map<size_t,std::vector<suffix_tree_node*>>& m){
    suffix_tree_node* x;
    std::vector<suffix_tree_node*> y;
    if(i< lenght_of_word - icfl_list[icfl_size-1]){
        x=add_suffix_in_node_sons_3(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i);
        size_t key=hash_substring(x->suffix,x->suffix_len);
        mut_map.lock();
        add_node_in_node_sons_5_map(m[key],x);
        mut_map.unlock();
    }
    for(int j=0;j<icfl_size-1;j++)
        add_node_in_suffix_tree_alberello_3(S,icfl_list,icfl_size,root,i,j,m);
}

void add_node_in_suffix_tree_alberello_3(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j,std::map<size_t,std::vector<suffix_tree_node*>>& m){
    suffix_tree_node* x;
    std::vector<suffix_tree_node*> y;
    if(i<icfl_list[j+1]-icfl_list[j]){
        x=add_suffix_in_node_sons_3(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,i+1,icfl_list[j]+icfl_list[j+1]-icfl_list[j]-1-i);
        size_t key=hash_substring(x->suffix,x->suffix_len);
        mut_map.lock();
        add_node_in_node_sons_5_map(m[key],x);
        mut_map.unlock();
    }
}