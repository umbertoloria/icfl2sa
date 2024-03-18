#include <iostream>
#include "generic_vector.h"
#include "custom_suffix_tree.h"
#include <string.h>
#include <vector>
#include "second_phase.h"
#include "utils.h"

using namespace std;


int_vector* find_common_elements_with_quick_sort(int_vector* x,int_vector* y){
    int_vector* res=init_int_vector(0);

    int_vector* x_temp = duplicate_int_vector(x);
    int_vector* y_temp = duplicate_int_vector(y);

    print_int_vector(x_temp);
    cout<<"\n";
    print_int_vector(y_temp);
    cout<<"\n";

    quicksort(x_temp,0,x_temp->used-1);
    quicksort(y_temp,0,y_temp->used-1);

    print_int_vector(x_temp);
    cout<<"\n";
    print_int_vector(y_temp);
    cout<<"\n";

    int i,j;
    i=j=0;

    while (i<x_temp->used && j<y_temp->used){
        if (x_temp->data[i] == y_temp->data[j]){
            add_in_int_vector(res,x_temp->data[i]);
            i++;
            j++;
        }
        else if(x_temp->data[i] < y_temp->data[j]){
            i++;
        }
        else{
            j++;
        }
    }
    
    print_int_vector(res);
    cout<<"\n";

    return res;
}

int_vector* find_common_elements(int_vector* x,int_vector* y){
    int_vector* res=init_int_vector(0);
    
    for(int i=0;i<x->used;i++){
        if(element_in_int_vector(y,x->data[i])){
            add_in_int_vector(res,x->data[i]);
        }
    }

    return res;
}

int_vector* find_common_elements_2(suffix_tree_node* x,suffix_tree_node* y){

    //cout<<x->suffix_len<<" "<<y->suffix_len<<"\n";

    while (x!=y){
        while (x->suffix_len!=y->suffix_len){
            
            while (x->suffix_len > y->suffix_len){
                x=x->father;
                //cout<<x->suffix_len<<" "<<y->suffix_len<<"\n";
            }

            while (x->suffix_len < y->suffix_len){
                y=y->father;
                //cout<<x->suffix_len<<" "<<y->suffix_len<<"\n";
            }
        }

        if(x!=y){
            x=x->father;
            y=y->father;
        }
    }
    

    
    
    return get_chain_from_bit_vector(x);
}

int_vector* common_prefix_merge(int_vector* x,int_vector* y){
    int_vector* res = init_int_vector(0);

    int_vector* common_elements = find_common_elements(x,y);

    int i,j,z;
    i=j=z=0;

    while(i<x->used){
        if(x->data[i] != common_elements->data[z]){
            add_in_int_vector(res,x->data[i]);
            i++;
        }
        else{
            while(y->data[j] != common_elements->data[z]){
                add_in_int_vector(res,y->data[j]);
                j++;
            }
            add_in_int_vector(res,common_elements->data[z]);
            z++;
            i++;
            j++;
        }
    }

    while(j<y->used){
        add_in_int_vector(res,y->data[j]);
        j++;
    }

    return res;

}


int_vector* common_prefix_merge_2(int_vector* x, suffix_tree_node* node1,int_vector* y, suffix_tree_node* node2){
    int_vector* res = init_int_vector(0);

    //cout<<"common elements: ";
    int_vector* common_elements = find_common_elements_2(node1,node2);
    //print_int_vector(common_elements);
    //cout<<"b\n";

    int i,j,z;
    i=j=z=0;

    while(i<x->used){
        if(x->data[i] != common_elements->data[z]){
            add_in_int_vector(res,x->data[i]);
            i++;
        }
        else{
            while(y->data[j] != common_elements->data[z]){
                add_in_int_vector(res,y->data[j]);
                j++;
            }
            add_in_int_vector(res,common_elements->data[z]);
            z++;
            i++;
            j++;
        }
    }

    while(j<y->used){
        add_in_int_vector(res,y->data[j]);
        j++;
    }

    return res;

}

//common_prefix_merge_3 effettua il common_prefix_merge però solo se si sanno a priori gli elementi comuni

int_vector* common_prefix_merge_3(int_vector* x,int_vector* y, int_vector*common_elements){
    int_vector* res = init_int_vector(x->used+y->used);
    //print_int_vector(common_elements);
    //cout<<"b\n";

    int i,j,z;
    i=j=z=0;
    

    while(i<x->used){
        if(z == common_elements->used) break;

        if(x->data[i] != common_elements->data[z]){
            add_in_int_vector(res,x->data[i]);
            i++;
        }
        else{
            while(y->data[j] != common_elements->data[z]){
                add_in_int_vector(res,y->data[j]);
                j++;
            }
            add_in_int_vector(res,common_elements->data[z]);
            z++;
            i++;
            j++;
        }
    }

    while(i<x->used){
        add_in_int_vector(res,x->data[i]);
        i++;
    }


    while(j<y->used){
        add_in_int_vector(res,y->data[j]);
        j++;
    }

    return res;

}

int_vector* common_prefix_merge_4(int_vector* x,int_vector* y, int_vector*common_elements){
    int_vector* res = init_int_vector(x->used+y->used);
    //print_int_vector(x);
    //print_int_vector(y);
    //print_int_vector(common_elements);
    //cout<<"b\n";

    int i,j,z;
    i=j=z=0;
    

    while(z != common_elements->used){
        while(x->data[i] != common_elements->data[z]) add_in_int_vector(res,x->data[i++]);
        i++;
        while(y->data[j] != common_elements->data[z]) add_in_int_vector(res,y->data[j++]);
        j++;
        add_in_int_vector(res,common_elements->data[z++]);
    }

    while(i<x->used) add_in_int_vector(res,x->data[i++]);
    while(j<y->used) add_in_int_vector(res,y->data[j++]);

    return res;

}

common_elements_vector* common_prefix_merge_with_common_elements_vector(int_vector* chain1,int_vector* common1,int_vector* chain2,int_vector* common2, int_vector*common_elements){
    common_elements_vector* res = init_common_elements_vector_3(chain1->used+chain2->used,common_elements->used);

    int tot_len,local_tot1,local_tot2,common1_distance,common2_distance,offset_elements;
    tot_len=local_tot1=local_tot2=0;
    
    for(int i=0;i<common_elements->used;i++){

        common1_distance=common1->data[i];
        memcpy(res->chain->data+tot_len,chain1->data+local_tot1,common1_distance * sizeof(int));
        local_tot1+=common1_distance + 1;
        tot_len+=common1_distance;

        common2_distance=common2->data[i];
        memcpy(res->chain->data+tot_len,chain2->data+local_tot2,common2->data[i] * sizeof(int));
        local_tot2+=common2->data[i] + 1;
        tot_len+=common2_distance;

        res->chain->data[tot_len]=common_elements->data[i];
        tot_len++;

        add_in_int_vector(res->distance_from_father,common1_distance+common2_distance);
    }

    if(local_tot1<chain1->used){
        offset_elements=chain1->used - local_tot1;
        memcpy(res->chain->data+tot_len,chain1->data+local_tot1,offset_elements * sizeof(int));
        tot_len+=offset_elements;
    }
    if(local_tot2<chain2->used){
        offset_elements=chain2->used - local_tot2;
        memcpy(res->chain->data+tot_len,chain2->data+local_tot2,offset_elements * sizeof(int));
        tot_len+=offset_elements;
    }

    res->chain->used=tot_len;


    return res;

}

int_vector* get_common_prefix_merge(suffix_tree_node* root){
    int_vector* res = init_int_vector(0);

    for(int i=0;i<root->chains_of_suffixes->used;i++){
        res = common_prefix_merge(res,root->chains_of_suffixes->data[i]);
    }
    return res;
}

int_vector* get_common_prefix_merge_2(suffix_tree_node* root){
    //print_array_of_vector(root->chains_of_suffixes);
    int_vector* res = get_chain_from_bit_vector(root->leaves->data[0]);
    //print_int_vector(res);

    for(int i=1;i<root->chains_of_suffixes->used;i++){
        res = common_prefix_merge_2(res,root->leaves->data[0],root->chains_of_suffixes->data[i],root->leaves->data[i]);
        //cout<<"parital result:";
        //print_int_vector(res);
    }
    //cout<<"result:";
    //print_int_vector(res);
    return res;
}

int_vector* get_common_prefix_merge_3(suffix_tree_node* root){
    if(root->sons->used==0){
        return get_chain_from_bit_vector(root);
    }

    int_vector* res = get_common_prefix_merge_3(root->sons->data[0]);
    int_vector* common_elements=get_chain_from_bit_vector(root);

    for(int i=1;i<root->sons->used;i++){
        res = common_prefix_merge_3(res,get_common_prefix_merge_3(root->sons->data[i]),common_elements);
    }
    //print_int_vector(res);
    return res;
}

//UTILIZZZA ARRAY DI APPOGGIO

int_vector* get_common_prefix_merge_4(suffix_tree_node* root){
    if(root->sons->used==0){
        //Senza array d'appoggio
        //return get_chain_from_bit_vector(root);
        //Con array d'appoggio
        return get_chain_from_bit_vector_3(root);
    }

    int_vector* res = get_common_prefix_merge_4(root->sons->data[0]);
    int_vector* temp_res;
    //Senza array d'appoggio
    //int_vector* common_elements=get_chain_from_bit_vector(root);
    //Con array d'appoggio
    int_vector* common_elements=get_chain_from_bit_vector_3(root);

    for(int i=1;i<root->sons->used;i++){
        int_vector* temp_son_common_prefix_merge = get_common_prefix_merge_4(root->sons->data[i]);
        temp_res = common_prefix_merge_4(res,temp_son_common_prefix_merge,common_elements);
        free(res->data);
        res=temp_res;
        free(temp_son_common_prefix_merge->data);
        free_node(root->sons->data[i]);
    }

    //free(common_elements->data);
    //print_int_vector(res);
    return res;
}

//usa i common elements
common_elements_vector* get_common_prefix_merge_5(suffix_tree_node* root){
    if(root->sons->used==0){
        cout<<"\n";
        root->common_elements_vec->chain=get_chain_from_common_elements_vector(root);
        cout<<"Risultato per nodo ";
        print_substring(root->suffix,root->suffix_len);
        cout<<": ";
        print_int_vector(root->common_elements_vec->chain);
        cout<<"distance_from_father: ";
        print_int_vector(root->common_elements_vec->distance_from_father);
        cout<<"\n";
        return root->common_elements_vec;
    }
;
    int_vector* temp_distance_from_father = duplicate_int_vector(root->common_elements_vec->distance_from_father);

    common_elements_vector* res = get_common_prefix_merge_5(root->sons->data[0]);

    cout<<"\n";

    cout<<"figlio ";
    print_substring(root->sons->data[0]->suffix,root->sons->data[0]->suffix_len);
    cout<<": ";
    print_int_vector(res->chain);
    cout<<"distance_from_father: ";
    print_int_vector(res->distance_from_father);

    common_elements_vector* temp_res;
    int_vector* common_elements=get_chain_from_common_elements_vector(root);

    for(int i=1;i<root->sons->used;i++){
        common_elements_vector* temp_son_common_prefix_merge = get_common_prefix_merge_5(root->sons->data[i]);
        cout<<"figlio ";
        print_substring(root->sons->data[i]->suffix,root->sons->data[i]->suffix_len);
        cout<<": ";
        print_int_vector(temp_son_common_prefix_merge->chain);
        cout<<"distance_from_father: ";
        print_int_vector(temp_son_common_prefix_merge->distance_from_father);
        temp_res = common_prefix_merge_with_common_elements_vector(res->chain,res->distance_from_father,temp_son_common_prefix_merge->chain,temp_son_common_prefix_merge->distance_from_father,common_elements);
        free(res->chain->data);
        free(res->bit_vec->data);
        free(res->distance_from_father->data);
        res=temp_res;
        //free_node_2(root->sons->data[i]);
    }


    free(common_elements->data);

    root->common_elements_vec->chain=res->chain;

    cout<<"Risultato per nodo ";
    print_substring(root->suffix,root->suffix_len);
    cout<<": ";
    print_int_vector(root->common_elements_vec->chain);
    cout<<"distance_from_father: ";
    print_int_vector(root->common_elements_vec->distance_from_father);
    //print_int_vector(res);

    cout<<"\n";

    return root->common_elements_vec;
}