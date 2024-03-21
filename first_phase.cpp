#include "first_phase.h"
#include "second_phase.h"


char** get_lyndon_words(string* word,vector<int> icfl_list){
    char** list_of_lyndon_words=(char**)malloc(sizeof(char*)*word->length());
    for(int i=0;i<icfl_list.size()-1;i++){
        const char* lyndon_word="\0";
        for (int j=icfl_list[i];j<icfl_list[i+1];j++){
            lyndon_word=append(lyndon_word,word->at(j));
        }
        list_of_lyndon_words[i]=strdup(lyndon_word);
    }
    //L'ultima parola di Lyndon
    const char* lyndon_word="\0";
    for(int i=icfl_list[icfl_list.size()-1];i<word->length();i++){
        lyndon_word=append(lyndon_word,word->at(i));
    }
    list_of_lyndon_words[icfl_list.size()-1]=strdup(lyndon_word);

    return list_of_lyndon_words;
}

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

suffix_tree_node* creazione_albero(char** list_of_lyndon_words,vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    for(int i=0;i<max_size;i++){
        //stampa_suffix_tree(root);
        //cout<<"\n\n";
        //cout<<i<<"/"<<max_size<<endl;
        nodes_vector* last_added_nodes=init_nodes_vector(0);
        suffix_tree_node* temp;
        //Viene elaborato prima l'ultima stringa
        const char* lyndon_word=list_of_lyndon_words[icfl_list.size()-1];
        //if(i<strlen(lyndon_word)){
        if(i< lenght_of_word - icfl_list[icfl_list.size()-1]){
            //La stringa si legge da destra verso sinistra
            //int starting_position= strlen(lyndon_word)-1-i;
            int starting_position= lenght_of_word - icfl_list[icfl_list.size()-1]-1-i;
            temp = add_suffix_in_tree_4(root,lyndon_word+starting_position,icfl_list[icfl_list.size()-1]+starting_position,i+1);
            if(temp){
                add_in_nodes_vector(last_added_nodes,temp);
            }
        }
        for(int j=0;j<icfl_list.size()-1;j++){
            const char* lyndon_word=list_of_lyndon_words[j];
            if(i<icfl_list[j+1]-icfl_list[j]){
                //La stringa si legge da destra verso sinistra
                int starting_position= icfl_list[j+1]-icfl_list[j]-1-i;
                temp = add_suffix_in_tree_4(root,lyndon_word+starting_position,icfl_list[j]+starting_position,i+1);
                if(temp){
                    add_in_nodes_vector(last_added_nodes,temp);
                }
            }
        }

        //ELABORA GLI ULTIMI NODI INSERITI
        //cout<<last_added_nodes->used<<endl;
        for(int i=0;i<last_added_nodes->used;i++){
            //cout<<"i: "<<i<<endl;
            create_bit_vector(S,icfl_list,last_added_nodes->data[i]);
            //cout<<last_added_nodes->data[i]->suffix<<endl;
            //print_bit_vector(last_added_nodes->data[i]->bit_vec);
        }

        free(last_added_nodes);
    }
    return root;
}

suffix_tree_node* creazione_albero_2(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_Size=icfl_list.size();
    for(int i=0;i<max_size;i++){
        //stampa_suffix_tree(root);
        //cout<<"\n\n";
        //cout<<i<<"/"<<max_size<<endl;
        nodes_vector* last_added_nodes=init_nodes_vector(icfl_Size);
        suffix_tree_node* temp;

        //if(i<strlen(lyndon_word)){
        if(i< lenght_of_word - icfl_list[icfl_Size-1]){
            //La stringa si legge da destra verso sinistra
            //int starting_position= strlen(lyndon_word)-1-i;
            int starting_position= lenght_of_word - icfl_list[icfl_Size-1]-1-i;
            temp = add_suffix_in_tree_4(root,S + icfl_list[icfl_Size-1] + starting_position,icfl_list[icfl_Size-1]+starting_position,i+1);
            if(temp) add_in_nodes_vector(last_added_nodes,temp);
        }
        for(int j=0;j<icfl_Size-1;j++){
            if(i<icfl_list[j+1]-icfl_list[j]){
                //La stringa si legge da destra verso sinistra
                int starting_position= icfl_list[j+1]-icfl_list[j]-1-i;
                temp = add_suffix_in_tree_4(root,S + icfl_list[j] +starting_position,icfl_list[j]+starting_position,i+1);
                if(temp) add_in_nodes_vector(last_added_nodes,temp);
            }
        }
        for(int i=0;i<last_added_nodes->used;i++)
            create_bit_vector_3_redundancy(S,icfl_list,icfl_Size,last_added_nodes->data[i]);

        free(last_added_nodes);
    }
    return root;
}

suffix_tree_node* creazione_albero_2_multithread(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_Size=icfl_list.size();
    clock_t tot_inserimento=0,tot_bitvector=0,tStart;
    for(int i=0;i<max_size;i++){
        //stampa_suffix_tree(root);
        //cout<<"\n\n";
        //cout<<i<<"/"<<max_size<<endl;

        tStart = clock();

        nodes_vector* last_added_nodes=init_nodes_vector(icfl_Size);
        suffix_tree_node* temp;

        //if(i<strlen(lyndon_word)){
        if(i< lenght_of_word - icfl_list[icfl_Size-1]){
            //La stringa si legge da destra verso sinistra
            //int starting_position= strlen(lyndon_word)-1-i;
            int starting_position= lenght_of_word - icfl_list[icfl_Size-1]-1-i;
            //temp = add_suffix_in_tree_4(root,S + icfl_list[icfl_Size-1] + starting_position,icfl_list[icfl_Size-1]+starting_position,i+1);
            add_suffix_in_tree_5(root,S + icfl_list[icfl_Size-1] + starting_position,icfl_list[icfl_Size-1]+starting_position,i+1,&temp);
            if(temp) add_in_nodes_vector(last_added_nodes,temp);
        }
        for(int j=0;j<icfl_Size-1;j++){
            if(i<icfl_list[j+1]-icfl_list[j]){
                //La stringa si legge da destra verso sinistra
                int starting_position= icfl_list[j+1]-icfl_list[j]-1-i;
                //temp = add_suffix_in_tree_4(root,S + icfl_list[j] +starting_position,icfl_list[j]+starting_position,i+1);
                add_suffix_in_tree_5(root,S + icfl_list[j] +starting_position,icfl_list[j]+starting_position,i+1,&temp);
                if(temp) add_in_nodes_vector(last_added_nodes,temp);
            }
        }

        tot_inserimento+=clock()-tStart;
        tStart = clock();


        std::thread gruppo_di_threads[last_added_nodes->used];
        for(int i=0;i<last_added_nodes->used;i++)
            gruppo_di_threads[i] = std::thread(create_bit_vector_3_redundancy, S,icfl_list,icfl_Size,last_added_nodes->data[i]);
        for(int i=0;i<last_added_nodes->used;i++)
            gruppo_di_threads[i].join();

        free(last_added_nodes);

        tot_bitvector+=clock()-tStart;
    }
    printf("tot_inserimento Time taken: %.2fs\n", (double)(tot_inserimento)/CLOCKS_PER_SEC);
    printf("tot_bitvector Time taken: %.2fs\n\n", (double)(tot_bitvector)/CLOCKS_PER_SEC);

    return root;
}

suffix_tree_node* creazione_albero_3_multithread(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_size=icfl_list.size();
    clock_t tot_inserimento=0,tot_bitvector=0,tStart;
    for(int i=0;i<max_size;i++){
        tStart = clock();

        compute_i_phase(S,lenght_of_word,icfl_list,icfl_size,root,i);
        //compute_i_phase_multithreding(S,lenght_of_word,icfl_list,icfl_size,root,i);

        tot_inserimento+=clock()-tStart;
    }

    tStart = clock();
    for(int i = 0;i<root->sons->used;i++)
        get_bit_vectors_from_root(S,icfl_list,icfl_size,root->sons->data[i]);
    tot_bitvector+=clock()-tStart;


    printf("tot_inserimento Time taken: %.2fs\n", (double)(tot_inserimento)/CLOCKS_PER_SEC);
    printf("tot_bitvector Time taken: %.2fs\n\n", (double)(tot_bitvector)/CLOCKS_PER_SEC);

    return root;
}

suffix_tree_node* creazione_albero_3(vector<int> icfl_list,const char* S,int lenght_of_word,int max_size){
    suffix_tree_node* root = build_suffix_tree_node(NULL,"\0",0);
    int icfl_Size=icfl_list.size();
    for(int i=0;i<max_size;i++){
        //stampa_suffix_tree(root);
        //cout<<"\n\n";
        //cout<<i<<"/"<<max_size<<endl;
        nodes_vector* last_added_nodes=init_nodes_vector(icfl_Size);
        suffix_tree_node* temp;

        //if(i<strlen(lyndon_word)){
        if(i< lenght_of_word - icfl_list[icfl_Size-1]){
            //La stringa si legge da destra verso sinistra
            //int starting_position= strlen(lyndon_word)-1-i;
            int starting_position= lenght_of_word - icfl_list[icfl_Size-1]-1-i;
            temp = add_suffix_in_tree_4(root,S + icfl_list[icfl_Size-1] + starting_position,icfl_list[icfl_Size-1]+starting_position,i+1);
            if(temp){
                add_in_nodes_vector(last_added_nodes,temp);
            }
        }
        for(int j=0;j<icfl_Size-1;j++){
            if(i<icfl_list[j+1]-icfl_list[j]){
                //La stringa si legge da destra verso sinistra
                int starting_position= icfl_list[j+1]-icfl_list[j]-1-i;
                temp = add_suffix_in_tree_4(root,S + icfl_list[j] +starting_position,icfl_list[j]+starting_position,i+1);
                if(temp){
                    add_in_nodes_vector(last_added_nodes,temp);
                }
            }
        }

        //ELABORA GLI ULTIMI NODI INSERITI
        //cout<<last_added_nodes->used<<endl;
        for(int i=0;i<last_added_nodes->used;i++){
            //cout<<"i: "<<i<<endl;
            //create_bit_vector_2(S,icfl_list,last_added_nodes->data[i]);
            //cout<<"Da inserire nodo: ";
            //print_substring(last_added_nodes->data[i]->suffix,last_added_nodes->data[i]->suffix_len);
            //cout<<"\n";
            create_bit_vector_4(S,icfl_list,icfl_Size,last_added_nodes->data[i]);
            cout<<"Nodo ";
            print_substring(last_added_nodes->data[i]->suffix,last_added_nodes->data[i]->suffix_len);
            cout<<"\nidici: ";
            print_int_vector(last_added_nodes->data[i]->array_of_indexes);
            cout<<"distance_from_father: ";
            print_int_vector(last_added_nodes->data[i]->common_elements_vec->distance_from_father);
    //print_int_vector(res);
            //cout<<"Inserito.\n";
            //cout<<last_added_nodes->data[i]->suffix<<endl;
            //print_bit_vector(last_added_nodes->data[i]->bit_vec);
        }

        free(last_added_nodes);
    }
    return root;
}

void compute_i_phase(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i){
    //I nodi nell'ultimo fattore, statisticamente e la maggior parte delle volte i< lenght_of_word - icfl_list[icfl_Size-1] è sempre vero, ma è sempre bene controllare che non si sa mai
    if(i< lenght_of_word - icfl_list[icfl_size-1])
        add_suffix_in_tree_4(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
    for(int j=0;j<icfl_size-1;j++)
        add_node_in_suffix_tree(S,icfl_list,icfl_size,root,i,j);
}

void compute_i_phase_multithreding(const char*S,int lenght_of_word,vector<int>icfl_list,int icfl_size,suffix_tree_node* root,int i){
    //I nodi nell'ultimo fattore, statisticamente e la maggior parte delle volte i< lenght_of_word - icfl_list[icfl_Size-1] è sempre vero, ma è sempre bene controllare che non si sa mai

    if(i< lenght_of_word - icfl_list[icfl_size-1])
        add_suffix_in_tree_4_multithreading(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
    for(int j=0;j<icfl_size-1;j++)
        std::thread(add_node_in_suffix_tree_multithreding,S,icfl_list,icfl_size,root,i,j).join();
    //if(i< lenght_of_word - icfl_list[icfl_size-1])
    //    add_suffix_in_tree_4_multithreading(root,S + icfl_list[icfl_size-1] + lenght_of_word - icfl_list[icfl_size-1]-1-i,icfl_list[icfl_size-1]+lenght_of_word - icfl_list[icfl_size-1]-1-i,i+1);
    //for(int j=0;j<icfl_size-1;j++){
    //    add_node_in_suffix_tree_multithreding(S,icfl_list,icfl_size,root,i,j);
    //}
}

//root,S + icfl_list[j] +starting_position,icfl_list[j]+starting_position,i+1
void add_node_in_suffix_tree(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j){
    if(i<icfl_list[j+1]-icfl_list[j]){
        add_suffix_in_tree_4(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,icfl_list[j]+icfl_list[j+1]-icfl_list[j]-1-i,i+1);
    }
}

void add_node_in_suffix_tree_multithreding(const char* S,vector<int> icfl_list,int icfl_size,suffix_tree_node* root,int i,int j){
    if(i<icfl_list[j+1]-icfl_list[j])
        add_suffix_in_tree_4_multithreading(root,S + icfl_list[j] +icfl_list[j+1]-icfl_list[j]-1-i,icfl_list[j]+icfl_list[j+1]-icfl_list[j]-1-i,i+1);
}