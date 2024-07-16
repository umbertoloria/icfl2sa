#include "custom_suffix_tree.h"
using namespace std;

std::unordered_map<size_t,std::mutex> mut_map_2;
std::mutex general_mutex;

unsigned long last_substring_in_map(const char *suffix,int suffix_len,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m){
    unsigned long key = 5381,last_key=0;
    int index,is_not_equal;

    for (int i=0;i<suffix_len;++i){
        key = ((key << 5) + key) + *(suffix+i); /* hash * 33 + c */
        //std::lock_guard<std::mutex> lock(mut_map_3[key]);
        //mut_map_3.lock();
        if(m.count(key)){
            index=binarySearch_4_with_redundancy(m.at(key),suffix,suffix_len,0,m.at(key).size()-1,&is_not_equal);
            if(!is_not_equal){
                last_key=key;
                //cout<<"trovato: "<<key<<"\n";
            }
        }
        //mut_map_3.unlock();
    }
    //cout<<"non trovato\n";
    return last_key;
}

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
    x->sons.reserve(1);
    x->array_of_indexes.reserve(1);
    x->common_chain_of_suffiexes.reserve(1);
    x->bit_vec.reserve(1);
    
    return x;
}

suffix_tree_node* build_suffix_tree_orphan_node(const char* suffix,int suffix_len){
    //suffix_tree_node* x= (suffix_tree_node*)malloc(sizeof(suffix_tree_node));
    suffix_tree_node* x = new(malloc(sizeof(suffix_tree_node))) suffix_tree_node{};

    x->father=NULL;
    x->suffix=suffix;

    x->suffix_len=suffix_len;

    return x;
}

int binarySearch_4_with_redundancy(vector<suffix_tree_node*> n_vector, const char* x,int suffix_len, int low, int high,int* is_not_equal) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista
    if(high==-1) return -1;
    int mid = (low + high) / 2;

    if(high == low){
        *is_not_equal = strncmp(x,n_vector[mid]->suffix,n_vector[mid]->suffix_len);
        return mid;
    }

    *is_not_equal=strncmp(x,n_vector[mid]->suffix,n_vector[mid]->suffix_len);

    if(*is_not_equal==0) return mid;

    if (*is_not_equal < 0) return binarySearch_4_with_redundancy(n_vector, x, suffix_len, low, mid,is_not_equal);

    return binarySearch_4_with_redundancy(n_vector, x, suffix_len, mid+1, high,is_not_equal);
}

int binarySearch_4_with_redundancy_2(nodes_vector* n_vector,int root_size, const char* x,int suffix_len, int low, int high,bool* is_equal) {
    
    //cout<<"low: "<<high<<", high: "<<low<<", mid: "<<mid<<"\n";
    //sleep(1);

    //Non ci sono elementi nella lista


    if(high==-1) return -1;
    int mid = (low + high) / 2;

    if(high == low){
        if(!strncmp(x+root_size,n_vector->data[mid]->suffix+root_size,n_vector->data[mid]->suffix_len-root_size))*is_equal=1;
        else *is_equal=0;
        return mid;
    }

    int res_of_strncmp=strncmp(x+root_size,n_vector->data[mid]->suffix+root_size,n_vector->data[mid]->suffix_len-root_size);

    if(res_of_strncmp==0){
        *is_equal=1;
        return mid;
    }

    if (res_of_strncmp < 0)
        return binarySearch_4_with_redundancy_2(n_vector,root_size, x, suffix_len, low, mid,is_equal);

    return binarySearch_4_with_redundancy_2(n_vector,root_size, x, suffix_len, mid+1, high,is_equal);
}


int binarySearch_4_with_redundancy_2_iterative(vector<suffix_tree_node*>& n_vector,int root_size, const char* x,int suffix_len, int low, int high,int* is_not_equal) {
    if(high==-1) return -1;
    int mid;
    while(high>=low){
        mid = (low + high) / 2;
        //print_substring(x,suffix_len);
        //cout<<"\n";
        //print_substring(n_vector[mid]->suffix,n_vector[mid]->suffix_len);
        //cout<<"\n";
        //cout<<"Root size: "<<root_size<<"\n";
        
        

        *is_not_equal = std::strncmp(x+root_size,n_vector[mid]->suffix+root_size,n_vector[mid]->suffix_len-root_size);

        //cout<<"is_not_equal: "<<*is_not_equal<<"\n";

        if(high == low || *is_not_equal==0) return mid;

        if (*is_not_equal < 0) high=mid;

        else low=mid+1;
    }
    return -1;
}

int binarySearch_4_with_redundancy_3_iterative(vector<suffix_tree_node*> n_vector,int root_size, const char* x,int suffix_len, int low, int high,int* is_not_equal) {
    if(high==-1) return -1;
    int mid,offset;
    char *s1,*s2;
    s1=(char*)malloc(sizeof(char)*suffix_len*suffix_len+4);
    s2=(char*)malloc(sizeof(char)*suffix_len*suffix_len+4);
    while(high>=low){
        mid = (low + high) / 2;
        offset = n_vector[mid]->suffix_len-root_size;
        
        //cout<<"A\n";
        memcpy(s1,x+root_size,offset);
        memcpy(s2,n_vector[mid]->suffix+root_size,offset);
        memset(s1+offset,'\0',4);
        memset(s2+offset,'\0',4);

        //cout<<"B\n";
        //cout<<string(s1)<<"\n";
        //cout<<string(s2)<<"\n";
        //cout<<"Suffix_len: "<<suffix_len<<"\n";
        //cout<<"Offset: "<<offset<<"\n";
        
        //*is_not_equal = strncmp(x+root_size,n_vector[mid]->suffix+root_size,n_vector[mid]->suffix_len-root_size);
        *is_not_equal = memcmp(s1,s2,offset);

        if(high == low || *is_not_equal==0) return mid;

        if (*is_not_equal < 0) high=mid;

        else low=mid+1;
    }
    return -1;
}

int linear_search(vector<suffix_tree_node*>& n_vector,int root_size, const char* x,int suffix_len){
    int index=-1;
    for(int i=0;i<n_vector.size();++i)
        if(!std::strncmp(x+root_size,n_vector[i]->suffix+root_size,n_vector[i]->suffix_len-root_size)){index=i;break;}
    return index;
}

void stampa_suffix_tree(suffix_tree_node* root){

    if (root->sons.empty()){
        cout<<"(";
        print_substring(root->suffix,root->suffix_len);
        cout<<"[";
        for(size_t j = 0; j<root->array_of_indexes.size();j++){
            cout<<root->array_of_indexes[j]<<",";
        }
        cout<<"])";
        return;
    }
    print_substring(root->suffix,root->suffix_len);
    cout<<"(";
    for(size_t i = 0; i<root->sons.size();i++){
        stampa_suffix_tree(root->sons[i]);
    }
    cout<<"[";
        for(size_t j = 0; j<root->array_of_indexes.size();j++){
            cout<<root->array_of_indexes[j]<<",";
        }
    cout<<"])";

    return;
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

void add_in_order_4(vector<suffix_tree_node*>& sons,suffix_tree_node* node,int starting_position){
    sons.push_back(NULL);
    std::move(sons.begin()+starting_position,sons.end()-starting_position,sons.begin()+starting_position+1);
    sons[starting_position]=node;
}

void add_in_order_5(vector<suffix_tree_node*>& sons,suffix_tree_node* node,int starting_position){
    sons.push_back(NULL);
    for(int j=sons.size()-1;j>starting_position;j--) sons[j] = sons[j-1];
    sons[starting_position]=node;
}

void add_in_order_6(vector<suffix_tree_node*>* sons,suffix_tree_node* node,int starting_position){
    (*sons).push_back(NULL);
    for(int j=(*sons).size()-1;j>starting_position;j--) (*sons)[j] = (*sons)[j-1];
    (*sons)[starting_position]=node;
}

void print_nodes_vector(nodes_vector* n_vec){
    for(int i=0;i<n_vec->used;i++){
        print_substring(n_vec->data[i]->suffix,n_vec->data[i]->suffix_len);
        cout<<", ";
    }
    cout<<"\n";
}

void print_nodes_vector_2(std::vector<suffix_tree_node*> n_vec){
    cout<<"(";
    for(int i=0;i<n_vec.size();i++){
        print_substring(n_vec[i]->suffix,n_vec[i]->suffix_len);
        cout<<", ";
    }
    cout<<")\n";
}

alberello* init_alberello(){
    alberello* x = (alberello*)malloc(sizeof(alberello));
    x->roots=init_nodes_vector(1);
    x->leaves=init_nodes_vector(1);
    return x;
}

void join_two_alberelli_3(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res){
    //int index,is_not_equal;
    std::map<intptr_t, std::mutex> map;
    suffix_tree_node* temp[b->sons.size()];
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    #pragma omp parallel for //shared(temp,a,b,map) schedule(static)
    for (int j=0;j<b->sons.size();++j){
        //temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        temp[j] = search_father_for_suffix_3_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len);
        map[(intptr_t)temp[j]].lock();
        add_node_in_node_sons_4(temp[j],b->sons[j]);
        map[(intptr_t)temp[j]].unlock();
    }
    //cout<<"finito\n";
    *res=a;
}

//Si differenzia dalla versione di prima per l'uso della binary search: viene usata solo nella ricerca del padre.
void join_two_alberelli_3_1(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res){
    int index,is_not_equal;
    std::map<intptr_t, std::mutex> map;
    suffix_tree_node* temp[b->sons.size()];
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    #pragma omp parallel for //shared(temp,a,b,map) schedule(static)
    for (int j=0;j<b->sons.size();++j){
        //temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        temp[j] = search_father_for_suffix_3_iterative_2(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        map[(intptr_t)temp[j]].lock();
        add_node_in_node_sons_5(temp[j],b->sons[j],index,is_not_equal);
        map[(intptr_t)temp[j]].unlock();
    }
    //cout<<"finito\n";
    *res=a;
}

void join_two_alberelli_3_2(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res){
    std::map<intptr_t, std::mutex> map;
    suffix_tree_node* temp[b->sons.size()];
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    #pragma omp parallel for //shared(temp,a,b,map) schedule(static)
    for (int j=0;j<b->sons.size();++j){
        //temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        temp[j] = search_father_for_suffix_3_iterative_3(a,b->sons[j]->suffix,b->sons[j]->suffix_len);
        map[(intptr_t)temp[j]].lock();
        temp[j]->sons.push_back(b->sons[j]);
        b->sons[j]->father=temp[j];
        //add_node_in_node_sons_5(temp[j],b->sons[j],index,is_not_equal);
        map[(intptr_t)temp[j]].unlock();
    }
    //cout<<"finito\n";
    *res=a;
}

//vechia implementazione senza std::vector<int>& is_custom_suffix
void join_two_alberelli_4(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res,const char* S,std::vector<int> icfl_list){
    //int index,is_not_equal;
    std::map<intptr_t, std::mutex> map;
    suffix_tree_node* temp[b->sons.size()];
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    #pragma omp parallel for shared(temp,a,b,map) schedule(static)
    for (int j=0;j<b->sons.size();++j){
        //temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        temp[j] = search_father_for_suffix_3_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len);
        map[(intptr_t)temp[j]].lock();
        add_node_in_node_sons_4(temp[j],b->sons[j]);
        //get_bit_vectors_from_root(S,icfl_list,icfl_list.size(),temp[j]);
        cout<<"Result:\n";
        printVec(b->sons[j]->common_chain_of_suffiexes);
        map[(intptr_t)temp[j]].unlock();
    }
    //cout<<"finito\n";
    *res=a;
}

suffix_tree_node* join_two_alberelli_4(suffix_tree_node* a,suffix_tree_node* b){
    int index,is_not_equal;
    suffix_tree_node* temp;
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    for (int j=0;j<b->sons.size();j++){
        temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        //cout<<"padre: ";
        //print_substring(temp->suffix,temp->suffix_len);
        //cout<<", figlio: ";
        //print_substring(b->sons[j]->suffix,b->sons[j]->suffix_len);
        //cout<<", index: "<<index;
        //cout<<"\n";
        add_node_in_node_sons_3(temp,b->sons[j],index,is_not_equal);
    }
    return a;
}

void join_two_alberelli_5(suffix_tree_node* a,suffix_tree_node* b,suffix_tree_node** res,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    //int index,is_not_equal;
    std::map<intptr_t, std::mutex> map;
    intptr_t key;
    suffix_tree_node* temp[b->sons.size()];
    //cout<<"num nodi da inserire: "<<b->sons.size()<<"\n";
    #pragma omp parallel for shared(temp,a,b,map,mutex_m) schedule(static)
    for (int j=0;j<b->sons.size();++j){
        //temp = search_father_for_suffix_2_iterative(a,b->sons[j]->suffix,b->sons[j]->suffix_len,&index,&is_not_equal);
        temp[j] = search_father_for_suffix_4(b->sons[j]->suffix,b->sons[j]->suffix_len-1,m,mutex_m);
        if(!temp[j]) temp[j]=a;
        key=(intptr_t)temp[j];
        map[key].lock();
        add_node_in_node_sons_4(temp[j],b->sons[j]);
        map[key].unlock();
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

void join_n_alberelli_omp(suffix_tree_node** roots,int k,suffix_tree_node** res_tree,const char* S,std::vector<int>& icfl_list){
    //suffix_tree_node** temp_res=(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*k);
    suffix_tree_node* temp_res[k];
    int use_temp=1;
    while (k>1){
        //cout<<"k: "<<k<<", use_temp: "<<use_temp<<"\n";
        if(use_temp%2) join_n_alberelli_omp_inner(roots,temp_res,&k,S,icfl_list);
        else join_n_alberelli_omp_inner(temp_res,roots,&k,S,icfl_list);
        ++use_temp;
    }
    
    *res_tree=roots[0];
}

void join_n_alberelli_omp_2(suffix_tree_node** roots,int k,suffix_tree_node** res_tree,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    //suffix_tree_node** temp_res=(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*k);
    suffix_tree_node* temp_res[k];
    int use_temp=1;
    while (k>1){
        //cout<<"k: "<<k<<", use_temp: "<<use_temp<<"\n";
        if(use_temp%2) join_n_alberelli_omp_inner_2(roots,temp_res,&k,m,mutex_m);
        else join_n_alberelli_omp_inner_2(temp_res,roots,&k,m,mutex_m);
        ++use_temp;
    }
    
    *res_tree=roots[0];
}

void join_n_alberelli_omp_inner(suffix_tree_node** roots,suffix_tree_node** temp_res,int* k,const char* S,std::vector<int>& icfl_list){
    #pragma omp parallel for //shared(roots,temp_res) schedule(static) // if(*k>1000) num_threads(std::thread::hardware_concurrency()/2)
    for(int i=0;i<*k/2;++i)
        join_two_alberelli_3_1(roots[i*2],roots[(i*2)+1],&temp_res[(i)]);
        //Versione sequenziale: 
        //join_two_alberelli_3_2(roots[i*2],roots[(i*2)+1],&temp_res[(i)]);

        //join_two_alberelli_4(roots[i*2],roots[(i*2)+1],&temp_res[(i)],S,icfl_list);
        //temp_res[(i)] = join_two_alberelli_4(roots[i*2],roots[(i*2)+1]);
    //for(int i=0;i<*k/2;++i){
    //    stampa_suffix_tree(temp_res[(i)]);
    //    cout<<"\n";
    //}
    
    //if(*k%2==1){ temp_res[*k/2]=roots[*k-1];stampa_suffix_tree(temp_res[(*k/2)]);cout<<"\n";*k=*k/2+1;}
    if(*k%2==1){ temp_res[*k/2]=roots[*k-1];*k=*k/2+1;}
    else *k=*k/2;
}

void join_n_alberelli_omp_inner_2(suffix_tree_node** roots,suffix_tree_node** temp_res,int* k,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    #pragma omp parallel for shared(roots,temp_res,mutex_m) schedule(static) // if(*k>1000) num_threads(std::thread::hardware_concurrency()/2)
    for(int i=0;i<*k/2;++i)
        join_two_alberelli_5(roots[i*2],roots[(i*2)+1],&temp_res[(i)],m,mutex_m);
        //temp_res[(i)] = join_two_alberelli_4(roots[i*2],roots[(i*2)+1]);
    //for(int i=0;i<*k/2;++i){
    //    stampa_suffix_tree(temp_res[(i)]);
    //    cout<<"\n";
    //}
    
    //if(*k%2==1){ temp_res[*k/2]=roots[*k-1];stampa_suffix_tree(temp_res[(*k/2)]);cout<<"\n";*k=*k/2+1;}
    if(*k%2==1){ temp_res[*k/2]=roots[*k-1];*k=*k/2+1;}
    else *k=*k/2;
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
    //int num_threads=4;
    cout<<"\nNum. of threads: "<<num_threads<<"\n";
    std::thread threads[num_threads];
    suffix_tree_node** temp_res;

    while (k>1){
        temp_res =(suffix_tree_node**)malloc(sizeof(suffix_tree_node*)*k/2);
        half=k/2;
        j=z=0;
        //cout<<"k: "<<k<<"\n";
        dim_chunks=half/num_threads;
        //if(dim_chunks)cout<<"numero chunk: "<<num_chunks<<"\n";
        //cout<<"dimensione chunk: "<<dim_chunks<<"\n";
        //cout<<"ciao\n";

        //Se c'è almeno una coppia da elaborare per ogni thread
        //sembra elaborare una stessa linea più volte di fila
        //so in generale non aggiorna l'albero
        if(dim_chunks){
            num_chunks=half/dim_chunks;
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
    for(;start<end;++start)
        join_two_alberelli_3(roots[start*2],roots[(start*2)+1],&res[(start)]);
}

void join_k_alberelli_2_openmp(suffix_tree_node** roots,suffix_tree_node** res,int start,int end){
    //cout<<"start: "<<start<<", end: "<<end<<"\n";
    int i;
    #pragma omp parallel for shared(roots,res) if(end>10000)//schedule(static, 10000)
    for(i=start;i<end;++i)
        join_two_alberelli_3(roots[i*2],roots[(i*2)+1],&res[(i)]);
    
}

suffix_tree_node* search_father_for_suffix_2(suffix_tree_node* root,const char* suffix,int suffix_len,int* index,int* is_not_equal){
    bool nuovo_nodo=false;
    *index = -1;


    //print_substring(root->suffix,root->suffix_len);
    //cout<<", ";
    //print_substring(suffix,suffix_len);
    //cout<<"\n";

    if(root->sons.empty()) return root;
    *index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,is_not_equal);
    //Ritorno se non è stato trovato un figlio buono per il suffisso
    if(*is_not_equal) return root;

    return search_father_for_suffix_2(root->sons[*index],suffix,suffix_len,index,is_not_equal);
}

suffix_tree_node* search_father_for_suffix_2_iterative(suffix_tree_node* root,const char* suffix,int suffix_len,int* index,int* is_not_equal){
    *index=-1;
    while (!root->sons.empty()){
        *index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,is_not_equal);
        if(*is_not_equal) return root;
        root=root->sons[*index];
    }
    return root;
}

suffix_tree_node* search_father_for_suffix_3_iterative(suffix_tree_node* root,const char* suffix,int suffix_len){
    int index,is_not_equal;
    while (!root->sons.empty()){
        //print_substring(suffix,suffix_len);
        //cout<<"\n";
        //print_nodes_vector_2(root->sons);
        //cout<<"\n";
        index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
        if(is_not_equal) return root;
        root=root->sons[index];
    }
    return root;
}

suffix_tree_node* search_father_for_suffix_3_iterative_2(suffix_tree_node* root,const char* suffix,int suffix_len,int* index,int* is_not_equal){
    while (!root->sons.empty()){
        *index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,is_not_equal);
        if(*is_not_equal) return root;
        root=root->sons[*index];
    }
    return root;
}

suffix_tree_node* search_father_for_suffix_3_iterative_3(suffix_tree_node* root,const char* suffix,int suffix_len){
    int index;
    while (!root->sons.empty()){
        index = linear_search(root->sons,root->suffix_len,suffix,suffix_len);
        if(index == -1) return root;
        root=root->sons[index];
    }
    return root;
}


suffix_tree_node* search_father_for_suffix_4(const char* suffix,int suffix_len,std::unordered_map<size_t,std::vector<suffix_tree_node*>>& m,std::mutex& mutex_m){
    suffix_tree_node* res=NULL;
    int is_not_equal,index;
    size_t key;
    //cout<<"Cercando padre per: ";
    //print_substring(suffix,suffix_len+1);
    //cout<<"\n";
    //ricerca sequenziale
    //for (;suffix_len>0;--suffix_len){
        //cout<<"ciao\n";
    //mut_map_2.lock();
    std::lock_guard<std::mutex> lock(mutex_m);
    key=last_substring_in_map(suffix,suffix_len,m);
    //print_substring(suffix,suffix_len);
    //cout<<" "<<key<<"\n";
    //cout<<m.count(key)<<"\n";
    //std::lock_guard<std::mutex> lock(mut_map_2[key]);
    if(m.count(key)){
        //cout<<" trovato con chiave: "<<key<<"\n";
        index=binarySearch_4_with_redundancy(m.at(key),suffix,suffix_len,0,m.at(key).size()-1,&is_not_equal);
        //cout<<" e index: "<<index<<"\n";
        if(!is_not_equal) res =m.at(key)[index];
    }
    //else cout<<"non trovato \n";
    //mut_map_2.unlock();
    //}
    //cout<<"non trovato\n";
    //cout<<"non trovato\n";
    return res;
}

//root è il nodo vuoto che contiene tutti i suffissi di lunghezza suffix_len
void add_suffix_in_node_sons_2(suffix_tree_node* root,const char* S,const char* suffix,int suffix_len,int suffix_index,vector<int>& icfl_list,vector<int>& custom_icfl_list,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list){
    int is_not_equal;
    //std::vector<int> temp_vec;
    suffix_tree_node* temp_root;
    //cout<<"Inserisco stringa: ";
    //print_substring(suffix,suffix_len);
    //cout<<"\n";

    //int index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
    int index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista

    //Se non è presente, prima di inserirlo inserisco il nodo in modo ordinato in ordine lessicografico
    if (root->sons.empty() || is_not_equal){
        suffix_tree_node* temp=build_suffix_tree_node(root,suffix,suffix_len);
        //cout<<"nuovo nodo\n";
        //cout<<"index "<<index<<" root->sons.size()-1 "<< root->sons.size()-1<<" is_not_equal "<<is_not_equal<<"\n";
        if(root->sons.empty() || (index == root->sons.size()-1 && is_not_equal>0))
            root->sons.push_back(temp);
        else
            //add_in_order_5(root->sons,temp,index);
            root->sons.insert(root->sons.begin() + index,temp);

        //Fine della creazione del nodo e del suo inserimento, lo inserisco all'interno dell' array dei suffissi

        //Se è un suffisso locale, lo inserico normalmente
        //cout<<suffix_index<<": "<<check_if_normal_index(icfl_list,lenght_of_word,suffix_index)<<"\n";

        temp_root=temp;
        //if(!is_custom_vec[suffix_index]) temp->array_of_indexes.push_back(suffix_index);
        ////altrimenti lo inseriso nella lista custom
        //else temp->custom_array_of_indexes.push_back(suffix_index);
    }
    else temp_root=root->sons[index];
        
        //cout<<suffix_index<<": "<<check_if_normal_index(icfl_list,lenght_of_word,suffix_index)<<"\n";
        //if(!is_custom_vec[suffix_index]) root->sons[index]->array_of_indexes.push_back(suffix_index);
        //else root->sons[index]->custom_array_of_indexes.push_back(suffix_index);
    //temp_vec.push_back(suffix_index);
    //temp_root->array_of_indexes=in_prefix_merge_bit_vector_7(S,icfl_list,icfl_list.size(),temp_root->array_of_indexes,temp_vec,is_custom_vec);
    in_prefix_merge_bit_vector_8(S,icfl_list,icfl_list.size(),temp_root->array_of_indexes,suffix_index,is_custom_vec,temp_root->suffix_len,factor_list);
    //temp_vec.clear();
} 

suffix_tree_node* add_suffix_in_node_sons_3(suffix_tree_node* root,const char* suffix,int suffix_len,int suffix_index){
    int is_not_equal;
    //cout<<"Inserisco stringa: ";
    //print_substring(suffix,suffix_len);
    //cout<<"\n";

    int index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista
    if (root->sons.empty() || is_not_equal){
        suffix_tree_node* temp=build_suffix_tree_node(root,suffix,suffix_len);
        //cout<<"nuovo nodo\n";
        //cout<<"index "<<index<<" root->sons.size()-1 "<< root->sons.size()-1<<" is_not_equal "<<is_not_equal<<"\n";
        if(root->sons.empty() || (index == root->sons.size()-1 && is_not_equal>0))
            root->sons.push_back(temp);
        else
            add_in_order_5(root->sons,temp,index);

        temp->array_of_indexes.push_back(suffix_index);
        return temp;
    }
    else root->sons[index]->array_of_indexes.push_back(suffix_index);
    return root->sons[index];
}

void add_suffix_in_node_sons_4(suffix_tree_node* root,const char* S,const char* suffix,int suffix_len,int suffix_index,vector<int>& icfl_list,vector<int>& custom_icfl_list,int lenght_of_word,vector<int>& is_custom_vec,vector<int>& factor_list){
    int is_not_equal;
    suffix_tree_node* temp_root;
    //int index = binarySearch_4_with_redundancy(root->sons,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
    int index = binarySearch_4_with_redundancy_2_iterative(root->sons,root->suffix_len,suffix,suffix_len,0,root->sons.size()-1,&is_not_equal);
    //Valuto solo se il suffisso che voglio inserire non è già presente all'interno della lista

    //Se non è presente, prima di inserirlo inserisco il nodo in modo ordinato in ordine lessicografico
    if (root->sons.empty() || is_not_equal){
        suffix_tree_node* temp=build_suffix_tree_node(root,suffix,suffix_len);
        if(root->sons.empty() || (index == root->sons.size()-1 && is_not_equal>0))
            root->sons.push_back(temp);
        else
            root->sons.insert(root->sons.begin() + index,temp);
        temp_root=temp;
    }
    else temp_root=root->sons[index];
        
    if(!is_custom_vec[suffix_index]) temp_root->array_of_indexes.push_back(suffix_index);
        //altrimenti lo inseriso nella lista custom
    else temp_root->custom_array_of_indexes.push_back(suffix_index);
} 

void add_node_in_node_sons_3(suffix_tree_node* opt_padre,suffix_tree_node* figlio,int index,int is_not_equal){
    if(opt_padre->sons.empty() || (index == opt_padre->sons.size()-1 && is_not_equal>0))
        opt_padre->sons.push_back(figlio);
    else
        add_in_order_5(opt_padre->sons,figlio,index);
    figlio->father=opt_padre;
}


void add_node_in_node_sons_4(suffix_tree_node* opt_padre,suffix_tree_node* figlio){
    int index,is_not_equal;
    index = binarySearch_4_with_redundancy_2_iterative(opt_padre->sons,opt_padre->suffix_len,figlio->suffix,figlio->suffix_len,0,opt_padre->sons.size()-1,&is_not_equal);    
    if(opt_padre->sons.empty() || (index == opt_padre->sons.size()-1 && is_not_equal>0))
        opt_padre->sons.push_back(figlio);
    else
        opt_padre->sons.insert(opt_padre->sons.begin()+ index,figlio);
        //add_in_order_5(opt_padre->sons,figlio,index);
    figlio->father=opt_padre;
}

void add_node_in_node_sons_5(suffix_tree_node* opt_padre,suffix_tree_node* figlio,int index,int is_not_equal){
    if(opt_padre->sons.empty() || (index == opt_padre->sons.size()-1 && is_not_equal>0))
        opt_padre->sons.push_back(figlio);
    else
        opt_padre->sons.insert(opt_padre->sons.begin()+ index,figlio);
        //add_in_order_5(opt_padre->sons,figlio,index);
    figlio->father=opt_padre;
}

void add_node_in_node_sons_5_map(std::vector<suffix_tree_node*>& opt_padre_sons,suffix_tree_node* figlio){
    int index,is_not_equal;
    //print_substring(figlio->suffix,figlio->suffix_len);
    //cout<<"\n";
    index = binarySearch_4_with_redundancy(opt_padre_sons,figlio->suffix,figlio->suffix_len,0,opt_padre_sons.size()-1,&is_not_equal);
    if(opt_padre_sons.empty()) opt_padre_sons.push_back(figlio);
    else if(!is_not_equal) return;
    else if(index == opt_padre_sons.size()-1 && is_not_equal>0) opt_padre_sons.push_back(figlio);
    else add_in_order_5(opt_padre_sons,figlio,index);
    //print_nodes_vector_2(opt_padre_sons);
    //printVec(opt_padre_sons[0]->array_of_indexes);
}

void merge_custom_array_of_indexes(const char* S,vector<int>& icfl_list,suffix_tree_node* alberello,std::vector<int> &is_custom_suffix, std::vector<int> &factor_list){
    for(int i=0;i < alberello->sons.size();i++){

        //print_substring(alberello->sons[i]->suffix,alberello->sons[i]->suffix_len);
        //cout<<"\n";
        //for(int j=0;j<alberello->sons[i]->array_of_indexes.size();j++){
        //    cout<<alberello->sons[i]->array_of_indexes[j]<<", ";
        //}
        //cout<<"\n";
        //for(int j=0;j<alberello->sons[i]->custom_array_of_indexes.size();j++){
        //    cout<<alberello->sons[i]->custom_array_of_indexes[j]<<", ";
        //}
        //cout<<"\n";

        
        //prima di inserirli ordino gli indici di fattori non canonici
        quicksort_of_indexes_2(S,alberello->sons[i]->custom_array_of_indexes,0,alberello->sons[i]->custom_array_of_indexes.size()-1,alberello->sons[i]->suffix_len);
        
        
        //inserisco gli indici ordinati all'interno del nodo corrispondete

        //li inserisco semplicemente in append
        //for(int j=0;j < alberello->sons[i]->custom_array_of_indexes.size();j++){
        //    alberello->sons[i]->array_of_indexes.push_back(alberello->sons[i]->custom_array_of_indexes[j]);
        //}

        //applico l'in_prefix_merge
        alberello->sons[i]->array_of_indexes = in_prefix_merge_bit_vector_9(S,icfl_list,icfl_list.size(),alberello->sons[i]->array_of_indexes,alberello->sons[i]->custom_array_of_indexes,is_custom_suffix,alberello->sons[i]->suffix_len,factor_list);
        
        //print_substring(alberello->sons[i]->suffix,alberello->sons[i]->suffix_len);
        //for(int j=0;j<alberello->sons[i]->array_of_indexes.size();j++){
        //    cout<<" "<<alberello->sons[i]->array_of_indexes[j]<<",";
        //}
        //cout<<"\n";
    }
}