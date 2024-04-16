#include<iostream>
#include<string.h>
#include "custom_suffix_tree.h"

#include "generic_vector.h"
#include "utils.h"

using namespace std;

std::mutex mut_map_3;

void print(string word){
    cout<<word<<endl;
}

void printVector(vector<int> vec, string msg){
    print(msg);
    for(int i=0; i<vec.size(); i++)
        cout<<vec.at(i)<<" ";
    cout<<endl;
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

unsigned long last_substring_in_map(const char *suffix,int suffix_len,std::map<size_t,std::vector<suffix_tree_node*>>& m){
    unsigned long key = 5381,last_key=0;
    int index,is_not_equal;

    for (int i=0;i<suffix_len;++i){
        key = ((key << 5) + key) + *(suffix+i); /* hash * 33 + c */
        //mut_map_3.lock();
        if(m.find(key) != m.end()){
            index=binarySearch_4_with_redundancy(m[key],suffix,suffix_len,0,m[key].size()-1,&is_not_equal);
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



