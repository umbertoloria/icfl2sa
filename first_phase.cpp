#include "first_phase.h"
#include "second_phase.h"

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
    
    clock_t tStart = clock();

    for(int i=0;i<max_size;i++)
        roots[i]=build_suffix_tree_node(NULL,"\0",0);

    //cout<<"inizializzati alberelli\n";

    //for(int i=0;i<max_size;i++)
    //    gruppo_di_threads[i] = std::thread(compute_i_phase_alberello_2,S,lenght_of_word,icfl_list,icfl_size,roots[i],i);

    //for(int i=0;i<max_size;i++)
    //    gruppo_di_threads[i].join();

    //cout<<"finito di computare\n";

    //#pragma omp parallel for schedule(static, 2)
    for(int i=0;i<max_size;i++)
        compute_i_phase_alberello_2(S,lenght_of_word,icfl_list,icfl_size,roots[i],i);

    printf("tot inizializzazione Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();

    //join_n_alberelli(roots,max_size,&root);
    //join_n_alberelli_multithreading(roots,max_size,&root);
    join_n_alberelli_multithreading_2(roots,max_size,&root);
    //join_n_alberelli_omp(roots,max_size,&root);

    printf("tot join Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //cout<<"finito il join\n";
    //stampa_suffix_tree(root);

    tStart = clock();
    for(int i = 0;i<root->sons.size();i++)
        get_bit_vectors_from_root(S,icfl_list,icfl_size,root->sons[i]);
    printf("get_bit_vectors_from_root Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
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


