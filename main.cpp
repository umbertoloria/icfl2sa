#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "factorizations.h"
#include "utils.h"
#include "custom_suffix_tree.h"
#include "first_phase.h"
#include "second_phase.h"
#include "third_phase.h"
#include "check.h"
#include <string.h>
#include <omp.h>

#define CONTROLLO_OUTPUT 1

using namespace std;

// STEP 1: COSTRUZIONE PREFIX TREE (in-prefix merge)


// sorting_suffixes_via_icfl_trie
vector<int> sorting_suffixes_via_icfl_trie(string* word,int lenght_of_word) {
    //cout<<"###################### sorting_suffixes_via_icfl_trie"<<endl;

    cout<<"Numero caratteri: "<<lenght_of_word<<endl;

    vector<int> icfl_list = ICFL_recursive(word, (*word).length());

    printVector(icfl_list, "Stampa ICFL");
    

    cout<<endl;
    
    int max_size=get_max_size(icfl_list,lenght_of_word);

    //cout<<"Max size: "<<max_size<<endl;

    double itime;

    itime = omp_get_wtime();

    suffix_tree_node* root = creazione_albero_alberelli(icfl_list,word->c_str(),lenght_of_word,max_size);


    printf("Creazione albero, Time taken: %.2fs\n", omp_get_wtime() - itime);

    //cout<<"\nALBERO OTTENUTO\n";
    //stampa_suffix_tree(root);
    
    if (!CONTROLLO_OUTPUT) word->clear();
    cout<<endl;
   
   clock_t tStart = clock();

    vector<int> group_ranking [root->sons.size()];
    //for(int i=0;i<root->sons->used;i++){
    //    //add_in_array_of_int_vector(group_ranking,get_common_prefix_merge_5(root->sons->data[i])->chain);
    //    add_in_array_of_int_vector(group_ranking,get_common_prefix_merge_4(root->sons->data[i]));
    //    //print_int_vector(get_common_prefix_merge_3(root->sons->data[i]));
    //}

    //cout<<"\nALBERO OTTENUTO\n";
    //stampa_suffix_tree(root);


    for(int i=0;i<root->sons.size();i++){
        group_ranking[i] = get_common_prefix_merge_4_multihreading_2(root->sons[i]);
        //cout<<"Group chain: ";
        //print_int_vector(group_ranking->data[i]);
        //cout<<"\n";
        
        //DA RE IMPLEMENTARE
        //add_in_array_of_int_vector(group_ranking,group_ranking->data[i]);
    }

    vector<int> SA;
    for(int i=0;i<root->sons.size();i++){
        SA.insert( SA.end(), group_ranking[i].begin(), group_ranking[i].end() );
    }


    //int_vector* SA = merge_array_of_vector_2(group_ranking,lenght_of_word);

    printf("Common+Concat, Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    if(CONTROLLO_OUTPUT){
        //cout<<"\n\n";
        //print_int_vector(SA);
        //cout<<"\n\n";
        tStart = clock();
        if(check_suffix_array(word->c_str(),SA)) cout<<"Il SA è valido."<<endl;
        printf("Testing, Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    }


    return icfl_list;

}

/*
// EXP 1: word given by user
void experiment_given_word() {
    //cout<<"###################### experiment_given_word"<<endl;

    string word;

    cout << "inserisci stringa: ";
    getline(cin, word);

    cout<<endl<<"Stringa: "<< word;
    cout << endl;

    vector<int> suffix_array = sorting_suffixes_via_icfl_trie(&word);

    return;
}

void experiment_given_word_by_cli(string word) {
    //cout<<"###################### experiment_given_word"<<endl;
    cout<<endl<<"Stringa: "<< word;
    cout << endl;

    vector<int> suffix_array = sorting_suffixes_via_icfl_trie(&word);

    return;
}

*/
char* experiment_given_word_by_input_file() {
    //cout<<"###################### experiment_given_word"<<endl;
    char * word = NULL;
    size_t len = 0;
    ssize_t read;

    FILE* fp = fopen("./input_file.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int word_size = getline(&word, &len, fp);

    //cout<<endl<<"Stringa: "<< word;
    //cout << endl;

    std::string x(word);

    vector<int> suffix_array = sorting_suffixes_via_icfl_trie(&x,word_size);

    return word;
}

int main(int argc, char** argv) {

    //experiment_given_word();
    //experiment_generate_word();

    //experiment_given_word_by_cli(string(argv[1]));

    clock_t tStart = clock();

    experiment_given_word_by_input_file();

    printf("\n\n Total time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    


    return 0;

}

