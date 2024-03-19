#include <malloc.h>
#include <iostream>
#include <string.h>
#include "generic_vector.h"

using namespace std;

int_vector* init_int_vector(size_t size){

    int_vector* x= (int_vector*)malloc(sizeof(int_vector));
    x->size=size;
    x->data=(int*)malloc(sizeof(int)*size);
    x->used=0;

    return x;

}

bool add_in_int_vector(int_vector* x,int element){
    if (x->size==x->used){
        x->size *=1000;
        x->data = (int*)realloc(x->data,sizeof(int)*x->size);
    }
    x->data[x->used++]=element;

    return 1;
}

bool element_in_int_vector(int_vector* x,int element){
    for(int i=0;i<x->used;i++) if(x->data[i]==element) return true;
    return false;
}

    
int_vector* duplicate_int_vector(int_vector* x){
    int_vector* temp = (int_vector*)malloc(sizeof(int_vector));
    temp->size=x->size;
    temp->used=x->used;
    temp->data=intdup(x->data,x->size);

    return temp;
}

array_of_int_vector* init_array_of_int_vector(size_t size){

    array_of_int_vector* x= (array_of_int_vector*)malloc(sizeof(array_of_int_vector));
    x->size=size;
    x->data=(int_vector**)malloc(sizeof(int_vector*)*size);
    x->used=0;

    return x;

}

bool add_in_array_of_int_vector(array_of_int_vector* x,int_vector* element){
    if (x->size==x->used){
        x->size += 1;
        x->data = (int_vector**)realloc(x->data,sizeof(int_vector*)*x->size);
    }
    x->data[x->used++]=element;

    return 1;
}


void print_int_vector(int_vector* x){

    for(int i=0;i<x->used;i++){
        cout<<x->data[i]<<",";
    }

    cout<<endl;

}

void print_array_of_vector(array_of_int_vector* x){

    for(int i=0;i<x->used;i++){
        cout<<"[\n";
        print_int_vector(x->data[i]);
        cout<<"]";
    }

    cout<<endl;

}

int_vector* merge_array_of_vector(array_of_int_vector* x){
    int_vector* res = init_int_vector(0);
    for(int i=0;i<x->used;i++){
        for(int j=0;j<x->data[i]->used;j++){
            add_in_int_vector(res,x->data[i]->data[j]);
        }
    }
    return res;
}

int_vector* merge_array_of_vector_2(array_of_int_vector* x,int size){
    int_vector* res = init_int_vector(size);
    for(int i=0;i<x->used;i++){
        for(int j=0;j<x->data[i]->used;j++){
            add_in_int_vector(res,x->data[i]->data[j]);
        }
    }
    return res;
}

int* intdup(int* src, size_t len){
   int* p = (int*)malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}

bit_vector* init_bit_vector(size_t size){
    bit_vector* x= (bit_vector*)malloc(sizeof(bit_vector));
    x->size=size;
    x->data=(bool*)malloc(sizeof(bool)*size);
    x->used=0;

    return x;

}

bool add_in_bit_vector(bit_vector* x,bool element){
        if (x->size==x->used){
        x->size *= 1000;
        x->data = (bool*)realloc(x->data,sizeof(bool)*x->size);
    }
    x->data[x->used++]=element;

    return 1;
}

void print_bit_vector(bit_vector* x){

    for(int i=0;i<x->used;i++){
        cout<<x->data[i]<<",";
    }

    cout<<endl;

}

common_elements_vector* init_common_elements_vector(){
    common_elements_vector* temp = (common_elements_vector*)malloc(sizeof(common_elements_vector));
    temp->chain=init_int_vector(0);
    temp->bit_vec=init_bit_vector(0);
    temp->distance_from_father=init_int_vector(0);

    return temp;
}

common_elements_vector* init_common_elements_vector_2(size_t size){
    common_elements_vector* temp = (common_elements_vector*)malloc(sizeof(common_elements_vector));
    temp->chain=init_int_vector(size);
    temp->bit_vec=init_bit_vector(size);
    temp->distance_from_father=init_int_vector(0);

    return temp;
}

common_elements_vector* init_common_elements_vector_3(size_t size,size_t size2){
    common_elements_vector* temp = (common_elements_vector*)malloc(sizeof(common_elements_vector));
    temp->chain=init_int_vector(size);
    temp->bit_vec=init_bit_vector(size);
    temp->distance_from_father=init_int_vector(size2);

    return temp;
}