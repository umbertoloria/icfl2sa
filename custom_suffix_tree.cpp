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
        x->size *= 2;
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

suffix_tree_node* build_suffix_tree_orphan_node(const char* suffix,int suffix_len){
    //suffix_tree_node* x= (suffix_tree_node*)malloc(sizeof(suffix_tree_node));
    suffix_tree_node* x = new(malloc(sizeof(suffix_tree_node))) suffix_tree_node{};

    x->father=NULL;
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
    
    if(root->suffix_len==suffix_len){
        root->node_lock.lock();
        add_in_int_vector(root->array_of_indexes,indice);
        root->node_lock.unlock();
        return;
    }

    bool nuovo_nodo=false;
    int index=-1;

    if(root->sons->used == 0)
        nuovo_nodo=true;
    
    else{
        index = binarySearch_3_with_redundancy(root,suffix,suffix_len,0,root->sons->used-1);
        if(strncmp(root->sons->data[index]->suffix,suffix,root->sons->data[index]->suffix_len))
            nuovo_nodo=true;
    }
    
    if(nuovo_nodo){
        root->node_lock.lock();
        suffix_tree_node* x=build_suffix_tree_node(root,suffix,suffix_len);
        add_in_int_vector(x->array_of_indexes,indice);
          
        if(index==-1) add_in_nodes_vector(root->sons,x);
        else if (strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0) add_in_nodes_vector(root->sons,x);
        else add_in_order_3(root->sons,x,index);
        x->father=root;
        
        root->node_lock.unlock();
        return ;
    }

    add_suffix_in_tree_4_multithreading(root->sons->data[index],suffix,indice,suffix_len);
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

int binarySearch_4_with_redundancy(nodes_vector* n_vector, const char* x,int suffix_len, int low, int high,bool* is_equal) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista
    if(high==-1) return -1;
    int mid = (low + high) / 2;

    if(high == low){
        if(!strncmp(x,n_vector->data[mid]->suffix,n_vector->data[mid]->suffix_len)) *is_equal=1;
        else *is_equal=0;
        return mid;
    }

    int res_of_strncmp=strncmp(x,n_vector->data[mid]->suffix,n_vector->data[mid]->suffix_len);

    if(res_of_strncmp==0){
        *is_equal=1;
        return mid;
    }

    if (res_of_strncmp < 0)
        return binarySearch_4_with_redundancy(n_vector, x, suffix_len, low, mid,is_equal);

    return binarySearch_4_with_redundancy(n_vector, x, suffix_len, mid+1, high,is_equal);
}

int binarySearch_4_with_redundancy_2(nodes_vector* n_vector, const char* x,int suffix_len, int low, int high,bool** is_equal) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista
    if(high==-1) return -1;
    int mid = (low + high) / 2;

    if(high == low){
        if(!strncmp(x,n_vector->data[mid]->suffix,n_vector->data[mid]->suffix_len)) **is_equal=1;
        else *is_equal=0;
        return mid;
    }

    int res_of_strncmp=strncmp(x,n_vector->data[mid]->suffix,n_vector->data[mid]->suffix_len);

    if(res_of_strncmp==0){
        **is_equal=1;
        return mid;
    }

    if (res_of_strncmp < 0)
        return binarySearch_4_with_redundancy_2(n_vector, x, suffix_len, low, mid,is_equal);

    return binarySearch_4_with_redundancy_2(n_vector, x, suffix_len, mid+1, high,is_equal);
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

void print_nodes_vector(nodes_vector* n_vec){
    for(int i=0;i<n_vec->used;i++){
        print_substring(n_vec->data[i]->suffix,n_vec->data[i]->suffix_len);
        cout<<", ";
    }
    cout<<"\n";
}

alberello* init_alberello(){
    alberello* x = (alberello*)malloc(sizeof(alberello));
    x->roots=init_nodes_vector(1);
    x->leaves=init_nodes_vector(1);
    return x;
}

//DA USARE SOLO QUANDO LA ROOTS È UGUALE ALLE FOGLIE
void add_in_alberello(alberello* alb,const char* suffix,int suffix_len){
    bool is_equal;
    int index = binarySearch_4_with_redundancy(alb->roots,suffix,suffix_len,0,alb->roots->used-1,&is_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista
    if (!alb->roots->used || !is_equal){
        //cout<<"nuovo nodo\n";
        if(!alb->roots->used || strcmp(alb->roots->data[alb->roots->used-1]->suffix,suffix)<0){
            add_in_nodes_vector(alb->roots,build_suffix_tree_orphan_node(suffix,suffix_len));
            add_in_nodes_vector(alb->leaves,build_suffix_tree_orphan_node(suffix,suffix_len));
        }
        else{
            add_in_order_3(alb->roots,build_suffix_tree_orphan_node(suffix,suffix_len),index);
            add_in_order_3(alb->leaves,build_suffix_tree_orphan_node(suffix,suffix_len),index);
        }
        
    }
    //else{
    //    cout<<"stringa già presente\n";
    //}
    //cout<<"Nuova taglia alberello: "<<alb->roots->used<<"\n";
}

void join_two_alberelli(suffix_tree_node* a,suffix_tree_node* b){
    int j=0,index;
    bool is_equal;
    suffix_tree_node* padre,*figlio;

    for (;j<b->sons->used;j++)
        add_node_in_node_sons(search_father_for_suffix(a,b->sons->data[j]->suffix,b->sons->data[j]->suffix_len),b->sons->data[j]);
}

void join_two_alberelli_2(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res){
    for (int j=0;j<b->sons->used;j++)
        add_node_in_node_sons(search_father_for_suffix(a,b->sons->data[j]->suffix,b->sons->data[j]->suffix_len),b->sons->data[j]);
    *res=a;
}

void join_two_alberelli_3(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res){
    int index;
    bool is_equal;
    suffix_tree_node* temp;
    //cout<<"num nodi da inserire: "<<b->sons->used<<"\n";
    for (int j=0;j<b->sons->used;j++){
        temp = search_father_for_suffix_2(a,b->sons->data[j]->suffix,b->sons->data[j]->suffix_len,&index,&is_equal);
        //cout<<"padre: ";
        //print_substring(temp->suffix,temp->suffix_len);
        //cout<<", figlio: ";
        //print_substring(b->sons->data[j]->suffix,b->sons->data[j]->suffix_len);
        //cout<<"\n";
        add_node_in_node_sons_2(temp,b->sons->data[j],index,is_equal);
    }
    //cout<<"finito\n";
    *res=a;
}

void join_n_alberelli(suffix_tree_node** roots,int k,suffix_tree_node** res_tree){
    while (k>1){
        for(int j=0;j<k/2;j++)
            join_two_alberelli_3(roots[j*2],roots[(j*2)+1],&roots[(j)]);
        if(k%2==1){ roots[k/2]=roots[k-1]; k=k/2+1;}
        else k=k/2;
    }
    *res_tree=roots[0];
}

void join_n_alberelli_multithreading(suffix_tree_node** roots,int k,suffix_tree_node** res_tree){
    int next_k,used_threads,temp_used_threads;
    int num_threads=std::thread::hardware_concurrency();
    cout<<"\nNum. of threads: "<<num_threads<<"\n";
    std::thread threads[num_threads];

    while (k>1){
        for(int j=0;j<k/2;j+=num_threads){
            for(used_threads=0;used_threads<num_threads && j+used_threads<k/2;used_threads++)
                threads[used_threads] = std::thread(join_two_alberelli_3,roots[(j+used_threads)*2],roots[((j+used_threads)*2)+1],&roots[(j+used_threads)]);
            //cout<<"ciao\n";
            for(temp_used_threads=0;temp_used_threads<used_threads;temp_used_threads++)
                threads[temp_used_threads].join();

            //cout<<"used_threads: "<<used_threads<<" temp_used_threads: "<<temp_used_threads<<"\n";
        }
        if(k%2==1){ roots[k/2]=roots[k-1]; k=k/2+1;}
        else k=k/2;
    }
    *res_tree=roots[0];
    

}

void join_n_alberelli_multithreading_2(suffix_tree_node** roots,int k,suffix_tree_node** res_tree){
    int j,z,num_chunks,dim_chunks,half,effective;
    int num_threads=std::thread::hardware_concurrency();
    
    cout<<"\nNum. of threads: "<<num_threads<<"\n";
    std::thread threads[num_threads];

    while (k>1){
        suffix_tree_node** temp_res =(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*k);
        half=k/2;
        j=z=0;
        //cout<<"k: "<<k<<"\n";
        dim_chunks=half/num_threads;
        if(dim_chunks)num_chunks=half/dim_chunks;
        //if(dim_chunks)cout<<"numero chunk: "<<num_chunks<<"\n";
        //cout<<"dimensione chunk: "<<dim_chunks<<"\n";
        //cout<<"ciao\n";

        //Se c'è almeno una coppia da elaborare per ogni thread
        //sembra elaborare una stessa linea più volte di fila
        //so in generale non aggiorna l'albero
        if(dim_chunks){
            //cout<<"multiplo\n";
            for(;j<num_threads;j++)
                threads[j] = std::thread(join_k_alberelli_2,roots,temp_res,j*dim_chunks,(j+1)*dim_chunks);
            //cout<<"ciao\n";
            for(;z<num_threads;z++)
                threads[z].join();
        }
        //else cout<<"singolo\n";

        //cout<<"manca da: "<<dim_chunks*used_threads<<" a: "<<k/2<<"\n";
        join_k_alberelli_2(roots,temp_res,dim_chunks*num_threads,half);
        //cout<<"fine multiplo...\n";

        if(k%2==1){ temp_res[half]=roots[k-1]; k=half+1;}
        else k=half;
        roots=temp_res;
    }
    *res_tree=roots[0];
}

void join_k_alberelli(suffix_tree_node** roots,int start,int end){
    //cout<<"start: "<<start<<", end: "<<end<<"\n";
    for(;start<end;start++)
        join_two_alberelli_3(roots[start*2],roots[(start*2)+1],&roots[(start)]);
}

void join_k_alberelli_2(suffix_tree_node** roots,suffix_tree_node** res,int start,int end){
    //cout<<"start: "<<start<<", end: "<<end<<"\n";
    for(;start<end;start++)
        join_two_alberelli_3(roots[start*2],roots[(start*2)+1],&res[(start)]);
}



suffix_tree_node* search_father_for_suffix(suffix_tree_node* root,const char* suffix,int suffix_len){
    bool nuovo_nodo=false,is_equal;
    int index=-1;

    if(root->sons->used == 0) return root;
    index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons->used-1,&is_equal);
    if(!is_equal) return root;

    return search_father_for_suffix(root->sons->data[index],suffix,suffix_len);
}

suffix_tree_node* search_father_for_suffix_2(suffix_tree_node* root,const char* suffix,int suffix_len,int* index,bool* is_equal){
    bool nuovo_nodo=false;
    *index = -1;


    //print_substring(root->suffix,root->suffix_len);
    //cout<<", ";
    //print_substring(suffix,suffix_len);
    //cout<<"\n";

    if(root->sons->used == 0) return root;
    *index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons->used-1,is_equal);
    //Ritorno se non è stato trovato un figlio buono per il suffisso
    if(!*is_equal) return root;

    return search_father_for_suffix_2(root->sons->data[*index],suffix,suffix_len,index,is_equal);
}

void add_suffix_in_node_sons(suffix_tree_node* root,const char* suffix,int suffix_len){
    bool is_equal;
    int index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons->used-1,&is_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista
    if (!root->sons->used || !is_equal){
        //cout<<"nuovo nodo\n";
        if(!root->sons->used || strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0)
            add_in_nodes_vector(root->sons,build_suffix_tree_node(root,suffix,suffix_len));
        else
            add_in_order_3(root->sons,build_suffix_tree_orphan_node(suffix,suffix_len),index);
    }
}

void add_suffix_in_node_sons_2(suffix_tree_node* root,const char* suffix,int suffix_len,int suffix_index){
    bool is_equal;
    int index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons->used-1,&is_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista
    if (!root->sons->used || !is_equal){
        suffix_tree_node* temp=build_suffix_tree_node(root,suffix,suffix_len);
        //cout<<"nuovo nodo\n";
        if(!root->sons->used || strcmp(root->sons->data[root->sons->used-1]->suffix,suffix)<0)
            add_in_nodes_vector(root->sons,temp);
        else
            add_in_order_3(root->sons,temp,index);

        add_in_int_vector(temp->array_of_indexes,suffix_index);
    }
    else add_in_int_vector(root->sons->data[index]->array_of_indexes,suffix_index);
}

//padre è il la posizione OPT dove inserire il nodo figlio
void add_node_in_node_sons(suffix_tree_node* opt_padre,suffix_tree_node* figlio){
    bool is_equal;
    int index = binarySearch_4_with_redundancy(opt_padre->sons,figlio->suffix,figlio->suffix_len,0,opt_padre->sons->used-1,&is_equal);
    if (!opt_padre->sons->used || !is_equal){
        if(!opt_padre->sons->used || strcmp(opt_padre->sons->data[opt_padre->sons->used-1]->suffix,figlio->suffix)<0)
            add_in_nodes_vector(opt_padre->sons,figlio);
        else
            add_in_order_3(opt_padre->sons,figlio,index);
    }
    figlio->father=opt_padre;
}

void add_node_in_node_sons_2(suffix_tree_node* opt_padre,suffix_tree_node* figlio,int index,bool is_equal){
    if (!opt_padre->sons->used || !is_equal){
        if(!opt_padre->sons->used || strcmp(opt_padre->sons->data[opt_padre->sons->used-1]->suffix,figlio->suffix)<0)
            add_in_nodes_vector(opt_padre->sons,figlio);
        else
            add_in_order_3(opt_padre->sons,figlio,index);
    }
    figlio->father=opt_padre;
}