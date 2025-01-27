
#include "utils.h"


using namespace std;

std::unordered_map<size_t,std::mutex> mut_map_3;

int pstrcmp( const void* a, const void* b )
{
  return strcmp( *(const char**)a, *(const char**)b );
}


void printUnorderedSet(std::unordered_set<int>& mySet) {
    for (const int& elem : mySet) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

void printDict(std::map<int,std::vector<int>>& index_to_nodes){

    for (const auto &[k, v] : index_to_nodes){
        std::cout << "m[" << k << "] = ";
        printVec(v);
        cout<<"\n";
    }

}

bool comparisonFunc(const char *c1, const char *c2){return strcmp(c1, c2) < 0;}

void print(string word){
    cout<<word<<endl;
}

void printVector(vector<int> vec, string msg){
    print(msg);
    for(int i=0; i<vec.size(); i++)
        cout<<vec.at(i)<<" ";
    cout<<endl;
}

void printBoolVec(vector<bool> vec){
    cout<<"(";
    for(int i=0; i<vec.size(); i++){
        if (vec.at(i)) cout<<"1 ";
        else cout<<"0 ";
    }
    cout<<")\n";
}

void printVec(vector<int> vec){
    cout<<"(";
    for(int i=0; i<vec.size(); i++)
        cout<<vec.at(i)<<" ";
    cout<<")\n";
}

char* append(const char *s, char c) {
    int len = strlen(s);
    char buf[len+2];
    strcpy(buf, s);
    buf[len] = c;
    buf[len + 1] = 0;
    return strdup(buf);
}

void quicksort(int_vector* x, int start, int end){
    int i, j, pivot, temp;
    if(start<end){

        pivot=start;
        
        i=start;
        j=end;     

        while(i<j){
            while(x->data[i]<=x->data[pivot]&&i<end) i++;
            while(x->data[j]>x->data[pivot]) j--;

            if(i<j){   
               temp=x->data[i];
               x->data[i]=x->data[j];
               x->data[j]=temp;
            }
        }

        temp=x->data[pivot];
        x->data[pivot]=x->data[j];
        x->data[j]=temp;
        quicksort(x,start,j-1);
        quicksort(x,j+1,end);
    }
}

void quicksort_of_nodes(nodes_vector* x, int start, int end){
    int i, j, pivot;
    suffix_tree_node* temp;
    if(start<end){

        pivot=start;
        
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
        quicksort_of_nodes(x,start,j-1);
        quicksort_of_nodes(x,j+1,end);
    }
}


void quicksort_of_indexes(const char* S,vector<int>& indexes, int start, int end){
    int i, j, pivot, temp;
    if(start<end){

        pivot=start;
        
        i=start;
        j=end;     

        while(i<j){
            while(strcmp(S+indexes[i],S+indexes[pivot])<=0 && i<end) i++;
            while(strcmp(S+indexes[j],S+indexes[pivot])>0) j--;

            if(i<j){   
               temp=indexes[i];
               indexes[i]=indexes[j];
               indexes[j]=temp;
            }
        }

        temp=indexes[pivot];
        indexes[pivot]=indexes[j];
        indexes[j]=temp;
        quicksort_of_indexes(S,indexes,start,j-1);
        quicksort_of_indexes(S,indexes,j+1,end);
    }
}

void quicksort_of_indexes_2(const char* S,vector<int>& indexes, int start, int end,int start_offset){
    int i, j, pivot, temp;
    
    if(start<end){
        
        pivot=start;
        //pivot = (rand()%(end-start))+start;
        //cout<<"pivot: "<<pivot<<", start: "<<start<<", end:"<<end<<"\n";
        
        i=start;
        j=end;     

        while(i<j){
            while(strcmp(S+indexes[i]+start_offset,S+indexes[pivot]+start_offset)<=0 && i<end) i++;
            while(strcmp(S+indexes[j]+start_offset,S+indexes[pivot]+start_offset)>0) j--;

            if(i<j){   
               temp=indexes[i];
               indexes[i]=indexes[j];
               indexes[j]=temp;
            }
        }

        temp=indexes[pivot];
        indexes[pivot]=indexes[j];
        indexes[j]=temp;
        quicksort_of_indexes_2(S,indexes,start,j-1,start_offset);
        quicksort_of_indexes_2(S,indexes,j+1,end,start_offset);
    }
}

void quicksort_of_indexes_3(const char* S,vector<int>& indexes, int start, int end,int start_offset,std::unordered_map<int,std::unordered_map<int,bool>*>& ord){
    int i, j, pivot, temp;
    int piccolo,grande;
    //decide se continuare a ciclare
    bool flag;
    
    if(start<end){
        
        pivot=start;
        //pivot = (rand()%(end-start))+start;
        //cout<<"pivot: "<<pivot<<", start: "<<start<<", end:"<<end<<"\n";
        
        i=start;
        j=end;     

        while(i<j){

            while(flag){
                if (i>=end) flag = false;
                else if(indexes[i] == indexes[pivot]) ++i;
                else if(indexes[i] > indexes[pivot]){
                    piccolo = indexes[pivot];
                    grande = indexes[i];
                    if(!(*ord[grande-piccolo])[piccolo]) flag = false;
                    else ++i;
                }
                else if(indexes[i] < indexes[pivot]){
                    grande = indexes[pivot];
                    piccolo = indexes[i];
                    if((*ord[grande-piccolo])[piccolo]) flag = false;
                    else ++i;
                }
            }
            flag=true;
            while(flag){
                if(indexes[j] == indexes[pivot]) flag = false;
                else if(indexes[j] > indexes[pivot]){
                    piccolo = indexes[pivot];
                    grande = indexes[j];
                    if((*ord[grande-piccolo])[piccolo]) flag = false;
                    else --j;
                }
                else if(indexes[j] < indexes[pivot]){
                    grande = indexes[pivot];
                    piccolo = indexes[j];
                    if(!(*ord[grande-piccolo])[piccolo]) flag = false;
                    else --j;
                }
            }

            if(i<j){   
               temp=indexes[i];
               indexes[i]=indexes[j];
               indexes[j]=temp;
            }
        }

        temp=indexes[pivot];
        indexes[pivot]=indexes[j];
        indexes[j]=temp;
        quicksort_of_indexes_3(S,indexes,start,j-1,start_offset,ord);
        quicksort_of_indexes_3(S,indexes,j+1,end,start_offset,ord);
    }
}

void quicksort_of_indexes_4(const char* S,vector<int>& indexes){
    int n_elements = indexes.size();
    std::vector<const char*> strings;
    for(int i=0;i<n_elements;++i) strings.push_back(S+indexes.at(i));
    std::sort(strings.begin(),strings.end(),comparisonFunc);
    for(int i=0;i<n_elements;++i) indexes.at(i)=(std::uintptr_t)strings.at(i)-(std::uintptr_t)S;
}

void quicksort_of_indexes_4_2(const char* S,vector<int>& indexes,int starting_offset){
    int n_elements = indexes.size();
    std::vector<const char*> strings;
    for(int i=0;i<n_elements;++i) strings.push_back(S+indexes.at(i)+starting_offset);
    std::sort(strings.begin(),strings.end(),comparisonFunc);
    for(int i=0;i<n_elements;++i) indexes.at(i)=((std::uintptr_t)strings.at(i)-starting_offset)-(std::uintptr_t)S;
}

//Questo è fortissimo
void quicksort_of_indexes_5(const char* S,vector<int>& indexes){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    int n_elements = indexes.size();
    const char** strings = (const char**)malloc(sizeof(*strings)*n_elements);
    for(int i=0;i<n_elements;++i) strings[i]=S+indexes.at(i);
    //for(int i=0;i<n_elements;++i) cout<<"S+i: "<<(std::uintptr_t)strings[i]<<"\n";;
    qsort(strings,n_elements,sizeof(strings[0]),pstrcmp);
    for(int i=0;i<n_elements;++i) indexes.at(i)=(std::uintptr_t)strings[i]-(std::uintptr_t)S;
    //printVec(indexes);
    //cout<<"\n";
}

void quicksort_of_indexes_5_2(const char* S,vector<int>& indexes,int starting_offset){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    int n_elements = indexes.size();
    const char** strings = (const char**)malloc(sizeof(*strings)*n_elements);
    for(int i=0;i<n_elements;++i) strings[i]=S+indexes.at(i)+starting_offset;
    //for(int i=0;i<n_elements;++i) cout<<"S+i: "<<(std::uintptr_t)strings[i]<<"\n";;
    qsort(strings,n_elements,sizeof(strings[0]),pstrcmp);
    for(int i=0;i<n_elements;++i) indexes.at(i)=((std::uintptr_t)strings[i]-starting_offset)-(std::uintptr_t)S;
    //printVec(indexes);
    //cout<<"\n";
}

void quicksort_of_indexes_5_3(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, vector<int>& indexes, int starting_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    if (indexes.empty()) return;
    std::sort(
        //std::execution::par,
        indexes.begin(), 
        indexes.end(),
        [S,&icfl_list,&icfl_list_size,&starting_offset,&is_custom_suffix,&factor_list,&indice_nodo](int x, int y){
            return custom_strcmp(S,icfl_list,icfl_list_size,x,y,starting_offset,is_custom_suffix,factor_list,indice_nodo) < 0;
        });
}

void quicksort_of_indexes_6(const char* S,vector<int>& indexes){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    int n_elements = indexes.size();
    if (!n_elements) return;
    const char** strings = (const char**)malloc(sizeof(*strings)*indexes.size());
    cout<<"Vector to order: \n";
    printVec(indexes);
    cout<<"\n";
    for(int i=0;i<n_elements;++i) strings[i]=S+indexes.at(i);
    //for(int i=0;i<n_elements;++i) cout<<"S+i: "<<(std::uintptr_t)strings[i]<<"\n";;
    radixsort(strings,n_elements);
    for(int i=0;i<n_elements;++i) indexes.at(i)=(std::uintptr_t)strings[i]-(std::uintptr_t)S;
    //printVec(indexes);
    //cout<<"\n";
}

void quicksort_of_indexes_7(const char* S,vector<int>& indexes,int lenght_of_word){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    int n_elements = indexes.size();
    if (!n_elements) return;
    const char** strings = (const char**)malloc(sizeof(*strings)*indexes.size());
    //cout<<"Vector to order: \n";
    //printVec(indexes);
    //cout<<"\n";
    for(int i=0;i<n_elements;++i) strings[i]=S+indexes.at(i);
    //for(int i=0;i<n_elements;++i) cout<<"S+i: "<<(std::uintptr_t)strings[i]<<"\n";;
    radixsort_msd(strings,n_elements,S,lenght_of_word);
    for(int i=0;i<n_elements;++i) indexes.at(i)=(std::uintptr_t)strings[i]-(std::uintptr_t)S;
    //cout<<"Ordered: \n";
    //printVec(indexes);
    //cout<<"\n";
}

void quicksort_of_indexes_8(const char* S,vector<int>& indexes){
    //cout<<"S: "<<(std::uintptr_t)S<<"\n";
    int n_elements = indexes.size();
    if (!n_elements) return;
    vector<const char*> strings;
    strings.resize(n_elements);
    //const char** strings = (const char**)malloc(sizeof(*strings)*indexes.size());
    for(int i=0;i<n_elements;++i) strings[i]=S+indexes.at(i);
    std::sort(strings.begin(), strings.end(), comparisonFunc);
    for(int i=0;i<n_elements;++i) indexes.at(i)=(std::uintptr_t)strings[i]-(std::uintptr_t)S;
}

void print_substring(const char* str,int n){
    for (int i = 0; i < n; ++i) {
        putchar(str[i]);
    }
}

char* get_substring(const char* basestring,int len){
    char* temp=(char*)malloc(sizeof(char)*len+1);
    memcpy(temp, basestring, len);
    temp[len] = '\0';
    return temp;
}

unsigned long get_hash_of_subsring(char* str){
    unsigned long res;
    //res = hash_substring(str);
    free(str);
    return res;
}

unsigned long hash_substring(const char *str,int size){
    unsigned long hash = 5381;
    int c;
    for (int i=0;i<size;++i)
        hash = ((hash << 5) + hash) + *str++; /* hash * 33 + c */
    return hash;
}

int fast_compare( const char *ptr0, const char *ptr1, int len ){
  int fast = len/sizeof(size_t) + 1;
  int offset = (fast-1)*sizeof(size_t);
  int current_block = 0;

  if( len <= sizeof(size_t)){ fast = 0; }


  size_t *lptr0 = (size_t*)ptr0;
  size_t *lptr1 = (size_t*)ptr1;

  while( current_block < fast ){
    if( (lptr0[current_block] ^ lptr1[current_block] )){
      int pos;
      for(pos = current_block*sizeof(size_t); pos < len ; ++pos ){
        if( (ptr0[pos] ^ ptr1[pos]) || (ptr0[pos] == 0) || (ptr1[pos] == 0) ){
          return  (int)((unsigned char)ptr0[pos] - (unsigned char)ptr1[pos]);
          }
        }
      }

    ++current_block;
    }

  while( len > offset ){
    if( (ptr0[offset] ^ ptr1[offset] )){ 
      return (int)((unsigned char)ptr0[offset] - (unsigned char)ptr1[offset]); 
      }
    ++offset;
    }
	
	
  return 0;
}

