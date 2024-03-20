#include <iostream>
#include <vector>
#include <malloc.h>
#include <string.h>
#include "custom_suffix_tree.h"
#include "utils.h"
using namespace std;

nodes_vector* init_nodes_vector(size_t size){
    nodes_vector* x= (nodes_vector*)malloc(sizeof(nodes_vector));
    x->size=size;
    x->data=(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*size);
    x->used=0;

    return x;
}

bool add_in_nodes_vector(nodes_vector* x,suffix_tree_node* element){
    if (x->size==x->used){
        x->size += 1;
        x->data = (suffix_tree_node**)realloc(x->data,sizeof(suffix_tree_node*)*x->size);
    }
    x->data[x->used++]=element;

    return 1;
}

//Costruttore del nodo prefix_tree_node

suffix_tree_node* build_suffix_tree_node(suffix_tree_node* father,const char* suffix,int suffix_len){
    //suffix_tree_node* x= (suffix_tree_node*)malloc(sizeof(suffix_tree_node));
    suffix_tree_node* x = new(malloc(sizeof(suffix_tree_node))) suffix_tree_node{};

    x->father=father;
    x->suffix=suffix;

    x->suffix_len=suffix_len;

    x->array_of_indexes = init_int_vector(1);
    x->sons=init_nodes_vector(1);
    x->leaves=init_nodes_vector(1);

    x->common_chain_of_suffiexes = init_int_vector(1);
    x->chains_of_suffixes = init_array_of_int_vector(1);

    x->common_elements_vec=init_common_elements_vector();

    return x;
}

void free_node(suffix_tree_node* root){
    free(root->array_of_indexes->data);
    free(root->bit_vec->data);
    free(root->chains_of_suffixes->data);
    free(root->common_chain_of_suffiexes->data);

    //free(root->common_elements_vec->chain->data);
    //free(root->common_elements_vec->bit_vec->data);
    //free(root->common_elements_vec->distance_from_father->data);
}

void free_node_2(suffix_tree_node* root){
    free(root->array_of_indexes->data);
    free(root->chains_of_suffixes->data);
    free(root->common_chain_of_suffiexes->data);

    free(root->common_elements_vec->chain->data);
    free(root->common_elements_vec->bit_vec->data);
    free(root->common_elements_vec->distance_from_father->data);
}


/*
La funzione aggiunge nel sottoalbero dettato da root il prefisso prefix.
Se viene trovato gli viene aggiunto l'indice alla lista degli indici.
Se non viene trovato viene creato nel punto opportuno (una foglia) e gli viene aggiunto l'indice alla lista.
IN: Root,Prefix,Indice
OUT: Bool (True se l'inserimento è andato a buon fine)
*/

suffix_tree_node* add_suffix_in_tree(suffix_tree_node* root,const char* suffix,int indice,int suffix_len){

    //Controlliamo se il suffisso che vogliamo inserire è contenuto nel nodo in cui ci troviamo

    if(strcmp(root->suffix,suffix)==0){
        add_in_int_vector(root->array_of_indexes,indice);
        //cout<<root->suffix<<","<<root->suffix_len<<"\n";
        //print_int_vector(root->array_of_indexes);
        return NULL;
    }

    /*
    Es. 
    Suffisso da inserire: abba
    Nuffisso presente nel nodo: ab
    */
    int16_t index_of_child_with_the_same_suffix = find_index_of_child_a_is_prefix_of_b(root,suffix);

    //Il prefisso non è in nessuno dei figli quindi prima creamo un figlio, lo aggiungiamo ai figli di root e gli inseriamo l'indice

    if(index_of_child_with_the_same_suffix == -1){
        //Creo un nuovo figlio del nodo root
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);
        add_in_nodes_vector(root->sons,x);
        x->father=root;
        //cout<<x->suffix<<","<<x->suffix_len<<"\n";
        //print_int_vector(x->array_of_indexes);
        return x;
    }

    // Il prefisso è contenuto in uno dei dei figli di root, ricorsivamente chiamiamo la funzione su tale figlio

    return add_suffix_in_tree(root->sons->data[index_of_child_with_the_same_suffix],suffix,indice,suffix_len);

}

//Usa ricerca binaria per inserimento e ricerca

suffix_tree_node* add_suffix_in_tree_2(suffix_tree_node* root,const char* suffix,int indice,int suffix_len){

    //cout<<suffix<<endl;

    if(strcmp(root->suffix,suffix)==0){
        add_in_int_vector(root->array_of_indexes,indice);
        return NULL;
    }
    
    //cout<<"Cercando...";
    int index_of_child_with_the_same_suffix = binarySearch(root,suffix,0,root->sons->used-1);
    //cout<<" trovato\n";

    if(index_of_child_with_the_same_suffix == -1){
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);
        //root->sons = add_in_order(root->sons,x);
        add_in_order_2(root->sons,x);
        x->father=root;
        //cout<<"sorting...";
        //quicksort_of_nodes_local(root->sons, 0, root->sons->used-1);
        //cout<<" done\n";

        return x;
    }
    
    return add_suffix_in_tree_2(root->sons->data[index_of_child_with_the_same_suffix],suffix,indice,suffix_len);

}

//Usa ricerca binaria per inserimento e ricerca, si salva i risultati intermedi

suffix_tree_node* add_suffix_in_tree_3(suffix_tree_node* root,const char* suffix,int indice,int suffix_len){

    if(strcmp(root->suffix,suffix)==0){
        add_in_int_vector(root->array_of_indexes,indice);
        return NULL;
    }
    
    //cout<<"Cercando...";
    //int index_of_child_with_the_same_suffix = binarySearch_2(root,suffix,0,root->sons->used-1);
    bool nuovo_nodo=false;
    int index=-1;

    if(root->sons->used == 0) nuovo_nodo=true;
    else{
        index = binarySearch_2_with_redundancy(root,suffix,suffix_len,0,root->sons->used-1);
        if(LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len) != root->sons->data[index]->suffix_len){
            nuovo_nodo=true;
            //cout<<"Non trovato suffisso simile a "<<suffix<<" at index: "<<index<<"\n";
            //cout<<"LCP: "<<LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len)<<", suffix_len: "<<root->sons->data[index]->suffix_len<<"\n";
            //for(int z=0;z<root->sons->used;z++) cout<<root->sons->data[z]->suffix<<", ";
            //cout<<"\n";
        }
    }
    //cout<<" trovato a "<<index_of_child_with_the_same_suffix<<"\n";

    if(nuovo_nodo){
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);

        //root->sons = add_in_order(root->sons,x);

        if(index==-1) add_in_nodes_vector(root->sons,x);
        else if (strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0) add_in_nodes_vector(root->sons,x);
        else add_in_order_3(root->sons,x,index);
        x->father=root;
        //cout<<"sorting...";
        //quicksort_of_nodes_local(root->sons, 0, root->sons->used-1);
        //cout<<" done\n";

        //cout<<"Inserito suffisso: "<<suffix<<"\n";
        //for(int z=0;z<root->sons->used;z++) cout<<root->sons->data[z]->suffix<<", ";
        //cout<<"\n";

        //cout<<"\n";
        //for(int z=0;z<root->sons->used;z++) cout<<root->sons->data[z]->suffix<<", ";
        //cout<<"\n";

        return x;
    }
    
    return add_suffix_in_tree_3(root->sons->data[index],suffix,indice,suffix_len);
}

suffix_tree_node* add_suffix_in_tree_4(suffix_tree_node* root,const char* suffix,int indice,int suffix_len){

    if(root->suffix_len==suffix_len){
        add_in_int_vector(root->array_of_indexes,indice);
        return NULL;
    }
    
    //cout<<"Cercando...";
    //int index_of_child_with_the_same_suffix = binarySearch_2(root,suffix,0,root->sons->used-1);
    bool nuovo_nodo=false;
    int index=-1;

    if(root->sons->used == 0) nuovo_nodo=true;
    else{
        index = binarySearch_3_with_redundancy(root,suffix,suffix_len,0,root->sons->used-1);
        if(strncmp(root->sons->data[index]->suffix,suffix,root->sons->data[index]->suffix_len)){
        //    cout<<"Non trovato suffisso simile a ";
        //    print_substring(suffix,suffix_len);
        //    cout<<" at index: "<<index<<"\n";
            nuovo_nodo=true;
            //cout<<"LCP: "<<LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len)<<", suffix_len: "<<root->sons->data[index]->suffix_len<<"\n";

        }
        //else{
        //    cout<<"Trovato suffisso simile a ";
        //    print_substring(suffix,suffix_len);
        //    cout<<" at index: "<<index<<"\n";
        //}
    }
    //cout<<" trovato a "<<index_of_child_with_the_same_suffix<<"\n";

    if(nuovo_nodo){
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);

        //root->sons = add_in_order(root->sons,x);

        if(index==-1) add_in_nodes_vector(root->sons,x);
        else if (strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0) add_in_nodes_vector(root->sons,x);
        else add_in_order_3(root->sons,x,index);
        x->father=root;

        //cout<<"LCP: "<<LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len)<<", suffix_len: "<<root->sons->data[index]->suffix_len<<"\n";
        //for(int z=0;z<root->sons->used;z++){
        //    print_substring(root->sons->data[z]->suffix,root->sons->data[z]->suffix_len);
        //    cout<<", ";
        //}
        //cout<<"\n";

        return x;
    }
    
    return add_suffix_in_tree_4(root->sons->data[index],suffix,indice,suffix_len);
}

void add_suffix_in_tree_4_multithreading(suffix_tree_node* root,const char* suffix,int indice,int suffix_len){

    //cout<<"Elaboro: ";
    //print_substring(suffix,suffix_len);
    //cout<<",Nel nodo: ";
    //print_substring(root->suffix,root->suffix_len);
    //cout<<"\n";
    //cout<<"\nLock: ";
    //if(root->node_lock.try_lock()) cout<<"true";
    //else cout<<"false";
    //cout<<"\n";

    //cout<<"GETTING LOCK OF: ";
    //print_substring(root->suffix,root->suffix_len);
    //cout<<"\n";
    root->node_lock.lock();
    //cout<<"\ndone\n";

    if(root->suffix_len==suffix_len){
        //cout<<"aggiorno indice array... \n";
        add_in_int_vector(root->array_of_indexes,indice);
        //cout<<"RELEASE LOCK OF: ";
        //print_substring(root->suffix,root->suffix_len);
        root->node_lock.unlock();
        //cout<<"\ndone\n";
        return;
    }

    bool nuovo_nodo=false;
    int index=-1;

    //cout<<"Elaboro sequenze..\n";
    if(root->sons->used == 0) //cout<<"1..\n";
        nuovo_nodo=true;
    
    else{
        //cout<<"2..\n";
        index = binarySearch_3_with_redundancy(root,suffix,suffix_len,0,root->sons->used-1);
        if(strncmp(root->sons->data[index]->suffix,suffix,root->sons->data[index]->suffix_len))
            nuovo_nodo=true;
    }

    //cout<<"Nuovo nodo: "<<nuovo_nodo<<"\n";
    if(nuovo_nodo){
        //cout<<"creo nuovo nodo... \n";
        
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        //cout<<"1\n";
        add_in_int_vector(x->array_of_indexes,indice);
        //cout<<"2\n";
          
        if(index==-1) add_in_nodes_vector(root->sons,x);
        else if (strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0) add_in_nodes_vector(root->sons,x);
        else add_in_order_3(root->sons,x,index);

        //cout<<"3\n";
        x->father=root;

        //cout<<"RELEASE LOCK OF: ";
        //print_substring(root->suffix,root->suffix_len);
        root->node_lock.unlock();
        //cout<<"\ndone\n";
        return ;
    }

    //cout<<"reitero\n";
    //cout<<"RELEASE LOCK OF: ";
    print_substring(root->suffix,root->suffix_len);
    root->node_lock.unlock();
    //cout<<"\ndone\n";
    add_suffix_in_tree_4_multithreading(root->sons->data[index],suffix,indice,suffix_len);
    //root->node_lock.unlock();
}

 void add_suffix_in_tree_5(suffix_tree_node* root,const char* suffix,int indice,int suffix_len,suffix_tree_node** res){

    if(root->suffix_len==suffix_len){
        add_in_int_vector(root->array_of_indexes,indice);
        *res = NULL;
        return;
    }
    
    //cout<<"Cercando...";
    //int index_of_child_with_the_same_suffix = binarySearch_2(root,suffix,0,root->sons->used-1);
    bool nuovo_nodo=false;
    int index=-1;

    if(root->sons->used == 0) nuovo_nodo=true;
    else{
        index = binarySearch_3_with_redundancy(root,suffix,suffix_len,0,root->sons->used-1);
        if(strncmp(root->sons->data[index]->suffix,suffix,root->sons->data[index]->suffix_len)){
        //    cout<<"Non trovato suffisso simile a ";
        //    print_substring(suffix,suffix_len);
        //    cout<<" at index: "<<index<<"\n";
            nuovo_nodo=true;
            //cout<<"LCP: "<<LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len)<<", suffix_len: "<<root->sons->data[index]->suffix_len<<"\n";

        }
        //else{
        //    cout<<"Trovato suffisso simile a ";
        //    print_substring(suffix,suffix_len);
        //    cout<<" at index: "<<index<<"\n";
        //}
    }
    //cout<<" trovato a "<<index_of_child_with_the_same_suffix<<"\n";

    if(nuovo_nodo){
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);

        //root->sons = add_in_order(root->sons,x);

        if(index==-1) add_in_nodes_vector(root->sons,x);
        else if (strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0) add_in_nodes_vector(root->sons,x);
        else add_in_order_3(root->sons,x,index);
        x->father=root;

        //cout<<"LCP: "<<LCP_with_given_strings_2(suffix,root->sons->data[index]->suffix,suffix_len)<<", suffix_len: "<<root->sons->data[index]->suffix_len<<"\n";
        //for(int z=0;z<root->sons->used;z++){
        //    print_substring(root->sons->data[z]->suffix,root->sons->data[z]->suffix_len);
        //    cout<<", ";
        //}
        //cout<<"\n";

        *res=x;
        return;
    }
    
    add_suffix_in_tree_5(root->sons->data[index],suffix,indice,suffix_len,res);
}



int binarySearch(suffix_tree_node* root, const char* x, int low, int high) {
  if (high >= low) {
    int mid = (low + high) / 2;

    // If found at mid, then return it
    //cout<<"LCP_with_given_strings?"<<endl;
    if(LCP_with_given_strings(x,root->sons->data[mid]->suffix) == root->sons->data[mid]->suffix_len){
      //cout<<"si"<<endl;
      return mid;
    }

    //cout<<"no"<<endl;

    // Search the left half
    if (strcmp(root->sons->data[mid]->suffix,x) > 0){
        return binarySearch(root, x, low, mid - 1);
    }

    // Search the right half
    return binarySearch(root, x, mid + 1, high);
  }

  return -1;
}

int binarySearch_with_redundancy(suffix_tree_node* root, const char* x, int suffix_len,int low, int high) {
  if (high >= low) {
    int mid = (low + high) / 2;

    // If found at mid, then return it
    //cout<<"LCP_with_given_strings?"<<endl;
    if(LCP_with_given_strings_2(x,root->sons->data[mid]->suffix,suffix_len) == root->sons->data[mid]->suffix_len){
      //cout<<"si"<<endl;
      return mid;
    }

    //cout<<"no"<<endl;

    // Search the left half
    if (strcmp(root->sons->data[mid]->suffix,x) > 0){
        return binarySearch_with_redundancy(root, x, suffix_len, low, mid - 1);
    }

    // Search the right half
    return binarySearch_with_redundancy(root, x, suffix_len, mid + 1, high);
  }

  return -1;
}

int binarySearch_2(suffix_tree_node* root, const char* x, int low, int high) {
    if (high >= low) {
        int mid = (low + high) / 2; 
        // If found at mid, then return it
        //cout<<"LCP_with_given_strings?"<<endl;
        if(high == low) return mid+1;

        if (strcmp(root->sons->data[mid]->suffix,x) > 0){
            return binarySearch_2(root, x, low, mid - 1);
        }

        return binarySearch_2(root, x, mid + 1, high);
    }
    if(high<0) return 0;
    else return high;
}

int binarySearch_2_with_redundancy(suffix_tree_node* root, const char* x,int suffix_len, int low, int high) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista
    if(high==-1) return -1;
    //cout<<"high: "<<high<<", low: "<<low<<", mid: "<<mid<<"\n";
    // If found at mid, then return it
    //cout<<"LCP_with_given_strings?"<<endl;
    //cout<<"Confronto: "<<root->sons->data[mid]->suffix<<", "<<x<<" at index: "<<mid<<"\n";

    int mid = (low + high) / 2; 

    if(high == low){
        return mid;
    }

    if(LCP_with_given_strings_2(x,root->sons->data[mid]->suffix,suffix_len) == root->sons->data[mid]->suffix_len)
        return mid;

    if (strcmp(root->sons->data[mid]->suffix,x) > 0){
        return binarySearch_2_with_redundancy(root, x, suffix_len, low, mid);
    }
    return binarySearch_2_with_redundancy(root, x, suffix_len, mid+1, high);
    
}

int binarySearch_3_with_redundancy(suffix_tree_node* root, const char* x,int suffix_len, int low, int high) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista
    if(high==-1) return -1;
    int mid = (low + high) / 2;

    //cout<<"high: "<<high<<", low: "<<low<<", mid: "<<mid<<"\n";
    // If found at mid, then return it
    //cout<<"LCP_with_given_strings?"<<endl;
    //cout<<"Confronto: ";
    //print_substring(root->sons->data[mid]->suffix,root->sons->data[mid]->suffix_len);
    //cout<<", ";
    //print_substring(x,suffix_len);
    //cout<<" at index: "<<mid<<"\n";


    if(high == low){
        return mid;
    }

    int res_of_strncmp=strncmp(x,root->sons->data[mid]->suffix,root->sons->data[mid]->suffix_len);

    if(res_of_strncmp==0)
        return mid;

    if (res_of_strncmp < 0){
        return binarySearch_3_with_redundancy(root, x, suffix_len, low, mid);
    }
    return binarySearch_3_with_redundancy(root, x, suffix_len, mid+1, high);
}

/*
Funzione che cerca il prefisso della stringa suffix passata in input tra i figli del nodo node passata input
Restituisce -1 se non è stato trovato.
IN: Nodo, Prefix
OUT: indice del figlio che ha lo stesso prefisso di Prefix 
*/

int16_t find_index_of_child_a_is_prefix_of_b(suffix_tree_node* node,const char* suffix){
    for(int16_t i=0;i<node->sons->size;i++){

            //SE E SOLO SE IL SUFFISSO È MAGGIORE O UGUALE AL SUFFISSO NEL FIGLIO DEL NODO
            //Questo perché se il figlio del nodo avesse una cardinalità maggiore, sicuramente non sarà prefisso.
            /*
            if(strlen(suffix) >= strlen(node->sons->data[i]->suffix)){
                if (std::string(suffix).compare(0,strlen(node->sons->data[i]->suffix),node->sons->data[i]->suffix) == 0){
                    return i;
                }
            }
            */
           //cout<<suffix<<endl<<node->sons->data[i]->suffix<<endl;
            //cout<<LCP_with_given_strings(suffix,node->sons->data[i]->suffix)<<endl<<strlen(suffix)<<endl;
            if(LCP_with_given_strings(suffix,node->sons->data[i]->suffix) == node->sons->data[i]->suffix_len){
                    //cout<<suffix<<endl<<node->sons->data[i]->suffix<<endl;
                    //cout<<LCP_with_given_strings(suffix,node->sons->data[i]->suffix)<<endl<<node->sons->data[i]->suffix_len<<endl;
                return i;
            }
    }
    return -1;
}


void stampa_suffix_tree(suffix_tree_node* root){

    if (root->sons->size==0){
        cout<<"(";
        print_substring(root->suffix,root->suffix_len);
        cout<<"[";
        for(size_t j = 0; j<root->array_of_indexes->used;j++){
            cout<<root->array_of_indexes->data[j]<<",";
        }
        cout<<"])";
        return;
    }
    print_substring(root->suffix,root->suffix_len);
    cout<<"(";
    for(size_t i = 0; i<root->sons->used;i++){
        stampa_suffix_tree(root->sons->data[i]);
    }
    cout<<"[";
        for(size_t j = 0; j<root->array_of_indexes->used;j++){
            cout<<root->array_of_indexes->data[j]<<",";
        }
    cout<<"])";

    return;
}

bool init_chains_of_prefixes(suffix_tree_node* root,int size_of_the_word){

    root->chains_of_suffixes = init_array_of_int_vector(0);

    for(int i=0;i<root->sons->used;i++){
        init_chains_of_prefixes(root->sons->data[i],size_of_the_word);
    }

    return 1;

}

int LCP(char* w, int index1,int index2){
    int i=0;
    while (w[index1+i]==w[index2+i]){
        i++;
    }
    return i;
}
int LCP_with_given_strings(const char* x,const char* y){
    int i=0;
    int max_len=strlen(x);
    while (x[i]==y[i] && i<max_len){
        i++;
    }
    return i;
}

int LCP_with_given_strings_2(const char* x,const char* y,int max_len){
    int i=0;
    while (x[i]==y[i] && i<max_len){
        i++;
    }
    return i;
}

int LCP_from_same_string(const char* piccola,const char* grande,int n){
    if(!strncmp(piccola,grande,n)) return n;
    return -1;
}

void quicksort_of_nodes_local(nodes_vector* x, int start, int end){
    int i, j, pivot;
    suffix_tree_node* temp;
    if(start<end){

        pivot=end;
        
        i=start;
        j=end;     

        while(i<j){
            while(strcmp(x->data[i]->suffix,x->data[pivot]->suffix)<=0 && i<end) i++;
            while(strcmp(x->data[j]->suffix,x->data[pivot]->suffix)>0) j--;

            if(i<j){   
               temp=x->data[i];
               x->data[i]=x->data[j];
               x->data[j]=temp;
            }
        }

        temp=x->data[pivot];
        x->data[pivot]=x->data[j];
        x->data[j]=temp;
        quicksort_of_nodes_local(x,start,j-1);
        quicksort_of_nodes_local(x,j+1,end);
    }
}

nodes_vector* add_in_order(nodes_vector* x,suffix_tree_node* node){
    nodes_vector* res=init_nodes_vector(0);
    int i=0;
    for(;i<x->used && strcmp(x->data[i]->suffix,node->suffix) <= 0;i++){
        add_in_nodes_vector(res,x->data[i]);
    }
    add_in_nodes_vector(res,node);
    for(;i<x->used;i++){
        add_in_nodes_vector(res,x->data[i]);
    }

    return res;

}
void add_in_order_2(nodes_vector* sons,suffix_tree_node* node){
    int i=0;
    while(i<sons->used && strcmp(sons->data[i]->suffix,node->suffix)<0) i++;
    add_in_nodes_vector(sons,NULL);
    for(int j=sons->used-1;j>i;j--) sons->data[j] = sons->data[j-1];
    sons->data[i]=node;
}

void add_in_order_3(nodes_vector* sons,suffix_tree_node* node,int starting_position){
    add_in_nodes_vector(sons,NULL);
    for(int j=sons->used-1;j>starting_position;j--) sons->data[j] = sons->data[j-1];
    sons->data[starting_position]=node;
}