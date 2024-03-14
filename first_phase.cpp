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
    if(lenght_of_word-icfl_list[icfl_list.size()-1] > max_size) return lenght_of_word-icfl_list[icfl_list.size()-1];
    return lenght_of_word;
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
            create_bit_vector_2(S,icfl_list,last_added_nodes->data[i]);
            //cout<<last_added_nodes->data[i]->suffix<<endl;
            //print_bit_vector(last_added_nodes->data[i]->bit_vec);
        }

        free(last_added_nodes);
    }
    return root;
}


