#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "factorizations.h"
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
vector<int> sorting_suffixes_via_icfl_trie(string* word,int lenght_of_word,int n_threads) {
    //cout<<"###################### sorting_suffixes_via_icfl_trie"<<endl;

    cout<<"Numero caratteri: "<<lenght_of_word<<endl;

    vector<int> icfl_list = ICFL_recursive(word, (*word).length());

    printVector(icfl_list, "Stampa ICFL");
    
    vector<int> custom_icfl_list = get_custom_factor_random(icfl_list,lenght_of_word);

    //printVector(custom_icfl_list, "Stampa custom_icfl_list");

    cout<<endl;
    
    int max_size=get_max_size(icfl_list,lenght_of_word);
    int custom_max_size=get_max_size(custom_icfl_list,lenght_of_word);

    cout<<"Max size: "<<max_size<<endl;
    cout<<"Custom Max size: "<<custom_max_size<<endl;

    double itime;

    itime = omp_get_wtime();
    custom_prefix_trie* root = creazione_albero_custom_prefix_trie(icfl_list,custom_icfl_list,word->c_str(),lenght_of_word,max_size,custom_max_size,n_threads);
    printf("creazione_albero_custom_prefix_trie, Time taken: %.2fs\n", omp_get_wtime() - itime);


    itime = omp_get_wtime();
    std::vector<std::vector<int>> group_ranking; 

    std::map<char,custom_prefix_trie*>::iterator it;
    for(it = root->sons.begin(); it != root->sons.end();++it)
        group_ranking.push_back(get_common_prefix_merge_4_multihreading_3(it->second,root->node->array_of_indexes));
    printf("get_common_prefix_merge_4_multihreading_3, Time taken: %.2fs\n", omp_get_wtime() - itime);

    std::vector<int> SA;
    for(int i=0;i<group_ranking.size();i++){
        SA.insert( SA.end(), group_ranking[i].begin(), group_ranking[i].end() );
    }

#if 0 
    printVec(SA);
#endif

    if(CONTROLLO_OUTPUT){
        itime = omp_get_wtime();
        if(check_suffix_array(word->c_str(),SA)) cout<<"Il SA è valido."<<endl;
        printf("check_suffix_array, Time taken: %.2fs\n", omp_get_wtime() - itime);
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
char* experiment_given_word_by_input_file(int n_threads) {
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

    vector<int> suffix_array = sorting_suffixes_via_icfl_trie(&x,word_size,n_threads);

    return word;
}

int main(int argc, char** argv) {
    int n_threads=1;
    srand(time(NULL));
    if(argc== 2) set_offset(atoi(argv[1]));
    else if(argc== 3){
        set_offset(atoi(argv[1]));
        n_threads=atoi(argv[2]);
    }
    cout<<"N. threads: "<<n_threads<<"\n";

    //experiment_given_word();
    //experiment_generate_word();

    //experiment_given_word_by_cli(string(argv[1]));

    //clock_t tStart = clock();
    double itime = itime = omp_get_wtime();

    experiment_given_word_by_input_file(n_threads);
    
    printf("\n\n Total time taken: %.2fs\n", omp_get_wtime() - itime);

    return 0;

}

