#include "merge.h"

int get_factor(vector<int> icfl_list,int index){
    int max_factor=icfl_list.size()-1;

    for(int i=0;i<max_factor;i++){
        if(index >= icfl_list.at(i) && index < icfl_list.at(i+1)){
            return i;
        }
    }

    return max_factor;

}

int_vector* in_prefix_merge(const char* S, vector<int> icfl_list, int_vector* e, int_vector* g){

    int_vector* result = init_int_vector(strlen(S));

    int i=0;
    int j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<e->used && j<g->used){

        int element_of_e=e->data[i];
        int element_of_g=g->data[j];

        //A

        if(element_of_e >= icfl_list[icfl_list.size()-1] && element_of_g >= icfl_list[icfl_list.size()-1]){
            add_in_int_vector(result,element_of_e);
            i++;
        }

        //B

        else if(get_factor(icfl_list,element_of_e)==get_factor(icfl_list,element_of_g)){
            add_in_int_vector(result,element_of_g);
            j++;
        }

        //C

        else{

            //1)

            if(element_of_e >= icfl_list[icfl_list.size()-1]){

                add_in_int_vector(result,element_of_e);
                i++;
            }

            //2)

            else if(element_of_g >= icfl_list[icfl_list.size()-1]){

                //cout<<"Secondo caso \n";

                //CONTROLLO LCP
                //LCP = Lunghezza dell suffisso del padre

                if(
                    S[element_of_g + LCP((char*)S,element_of_e,element_of_g)] 
                    <
                    S[element_of_e + LCP((char*)S,element_of_e,element_of_g)]){

                        //cout<<element_of_e<<endl<<element_of_g<<endl;
                        //cout<<S+element_of_e<<endl<<S+element_of_g<<endl<<LCP((char*)S,element_of_e,element_of_g)<<endl;
                    
                    
                        add_in_int_vector(result,element_of_g);
                        j++;
                }

                else{
                    add_in_int_vector(result,element_of_e);
                    i++;
                }
            }

            //3)

            else{

                if(element_of_e > element_of_g){
                    add_in_int_vector(result,element_of_g);
                    j++;
                }

                else{

                    if(S[element_of_g + LCP((char*)S,element_of_e,element_of_g)] < S[element_of_e + LCP((char*)S,element_of_e,element_of_g)]){
                        add_in_int_vector(result,element_of_g);
                        j++;
                    }

                    else{
                        add_in_int_vector(result,element_of_e);
                        i++;
                    }

                }

            }
        }

    }

    while(j<g->used){
        add_in_int_vector(result,g->data[j]);
        j++;
    }

    while(i<e->used){
        add_in_int_vector(result,e->data[i]);
        i++;
    }

    return result;
}

bit_vector* in_prefix_merge_bit_vector(const char* S, vector<int> icfl_list, int_vector* father, int_vector* child){

    //FATHER = 1
    //CHILD = 0

    bit_vector* result = init_bit_vector(strlen(S));

    int i=0;
    int j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father->used && j<child->used){

        int element_of_e=father->data[i];
        int element_of_g=child->data[j];

        //A

        if(element_of_e >= icfl_list[icfl_list.size()-1] && element_of_g >= icfl_list[icfl_list.size()-1]){
            add_in_bit_vector(result,true);
            i++;
        }

        //B

        else if(get_factor(icfl_list,element_of_e)==get_factor(icfl_list,element_of_g)){
            add_in_bit_vector(result,false);
            j++;
        }

        //C

        else{

            //1)

            if(element_of_e >= icfl_list[icfl_list.size()-1]){

                add_in_bit_vector(result,true);
                i++;
            }

            //2)

            else if(element_of_g >= icfl_list[icfl_list.size()-1]){

                //cout<<"Secondo caso \n";

                //CONTROLLO LCP
                //LCP = Lunghezza dell suffisso del padre

                if(
                    S[element_of_g + LCP((char*)S,element_of_e,element_of_g)] 
                    <
                    S[element_of_e + LCP((char*)S,element_of_e,element_of_g)]){

                        //cout<<element_of_e<<endl<<element_of_g<<endl;
                        //cout<<S+element_of_e<<endl<<S+element_of_g<<endl<<LCP((char*)S,element_of_e,element_of_g)<<endl;
                    
                    
                        add_in_bit_vector(result,false);
                        j++;
                }

                else{
                    add_in_bit_vector(result,true);
                    i++;
                }
            }

            //3)

            else{

                if(element_of_e > element_of_g){
                    add_in_bit_vector(result,false);
                    j++;
                }

                else{

                    if(S[element_of_g + LCP((char*)S,element_of_e,element_of_g)] < S[element_of_e + LCP((char*)S,element_of_e,element_of_g)]){
                        add_in_bit_vector(result,false);
                        j++;
                    }

                    else{
                        add_in_bit_vector(result,true);
                        i++;
                    }

                }

            }
        }

    }

    while(j<child->used){
        add_in_bit_vector(result,false);
        j++;
    }

    while(i<father->used){
        add_in_bit_vector(result,true);
        i++;
    }

    return result;
}

bit_vector* in_prefix_merge_bit_vector_2(const char* S, vector<int> icfl_list, int_vector* father, int_vector* child){
    int icfl_list_size=icfl_list.size();
    //FATHER = 1
    //CHILD = 0

    bit_vector* result = init_bit_vector(father->used+child->used);

    int i=0;
    int j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father->used && j<child->used){

        int element_of_e=father->data[i];
        int element_of_g=child->data[j];

        //A

        if(element_of_e >= icfl_list[icfl_list_size-1] && element_of_g >= icfl_list[icfl_list_size-1]){
            add_in_bit_vector(result,true);
            i++;
        }

        //B

        else if(get_factor(icfl_list,element_of_e)==get_factor(icfl_list,element_of_g)){
            add_in_bit_vector(result,false);
            j++;
        }

        //C

        else{

            //1)

            if(element_of_e >= icfl_list[icfl_list_size-1]){

                add_in_bit_vector(result,true);
                i++;
            }

            //2)

            else if(element_of_g >= icfl_list[icfl_list_size-1]){
                
                if(strcmp(S+element_of_g,S+element_of_e)<0){
                    add_in_bit_vector(result,false);
                    j++;
                }

                else{
                    add_in_bit_vector(result,true);
                    i++;
                }
            }

            //3)

            else{

                if(element_of_e > element_of_g){
                    add_in_bit_vector(result,false);
                    j++;
                }

                else{

                    if(strcmp(S+element_of_g,S+element_of_e)<0){
                        add_in_bit_vector(result,false);
                        j++;
                    }

                    else{
                        add_in_bit_vector(result,true);
                        i++;
                    }

                }

            }
        }

    }

    while(j<child->used){
        add_in_bit_vector(result,false);
        j++;
    }

    while(i<father->used){
        add_in_bit_vector(result,true);
        i++;
    }

    return result;
}

bit_vector* in_prefix_merge_bit_vector_3(const char* S, vector<int> icfl_list, int icfl_list_size, int_vector* father, int_vector* child,int common_prefix_len){
    //common_prefix_len=0;
    //FATHER = 1
    //CHILD = 0

    bit_vector* result = init_bit_vector(father->used+child->used);

    int i=0;
    int j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father->used && j<child->used){


        int element_of_e=father->data[i];
        int element_of_g=child->data[j];

        //cout<<"\nConfrontando: "<<element_of_e<<" e "<<element_of_g<<" , fattore: "<<get_factor(icfl_list,element_of_e)<<" e "<<get_factor(icfl_list,element_of_g)<<"\n";

        //A

        if(element_of_e >= icfl_list[icfl_list_size-1] && element_of_g >= icfl_list[icfl_list_size-1]){
            add_in_bit_vector(result,true);
            i++;
            //cout<<"\nA\n";
        }

        //B

        else if(get_factor(icfl_list,element_of_e)==get_factor(icfl_list,element_of_g)){
            add_in_bit_vector(result,false);
            j++;
            //cout<<"\nB\n";
        }

        //C

        else{

            //1)

            if(element_of_e >= icfl_list[icfl_list_size-1]){
                add_in_bit_vector(result,true);
                i++;
                //cout<<"\nC1\n";
            }

            //2)

            else if(element_of_g >= icfl_list[icfl_list_size-1]){

                //cout<<S+element_of_e+common_prefix_len<<"\n";
                //cout<<S+element_of_g+common_prefix_len<<"\n";
                
                if(strcmp(S+element_of_g+common_prefix_len,S+element_of_e+common_prefix_len)<0){
                    add_in_bit_vector(result,false);
                    j++;
                    //cout<<"\nC2.1\n";
                }

                else{
                    add_in_bit_vector(result,true);
                    i++;
                    //cout<<"\nC2.2\n";
                }
            }

            //3)

            else{

                if(element_of_e > element_of_g){
                    add_in_bit_vector(result,false);
                    j++;
                    //cout<<"\nC3.1\n";
                }

                else{

                    if(strcmp(S+element_of_g+common_prefix_len,S+element_of_e+common_prefix_len)<0){
                        add_in_bit_vector(result,false);
                        j++;
                        //cout<<"\nC3.2.1\n";
                    }

                    else{
                        add_in_bit_vector(result,true);
                        i++;
                        //cout<<"\nC3.2.2\n";
                    }

                }

            }
        }

    }

    while(j<child->used){
        add_in_bit_vector(result,false);
        j++;
    }

    while(i<father->used){
        add_in_bit_vector(result,true);
        i++;
    }

    return result;
}

common_elements_vector* in_prefix_merge_bit_vector_4(const char* S, vector<int> icfl_list,int icfl_list_size,int_vector* father, int_vector* child){

    common_elements_vector* result = init_common_elements_vector_2(father->used+child->used);
    //FATHER = 1
    //CHILD = 0


    int i=0;
    int j=0;
    int until_next_common_element=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father->used && j<child->used){

        int element_of_e=father->data[i];
        int element_of_g=child->data[j];

        //A

        if(element_of_e >= icfl_list[icfl_list_size-1] && element_of_g >= icfl_list[icfl_list_size-1]){
            add_in_bit_vector(result->bit_vec,true);
            i++;
            add_in_int_vector(result->distance_from_father,until_next_common_element);
            until_next_common_element=0;
        }

        //B

        else if(get_factor(icfl_list,element_of_e)==get_factor(icfl_list,element_of_g)){
            add_in_bit_vector(result->bit_vec,false);
            j++;
            until_next_common_element++;
        }

        //C

        else{

            //1)

            if(element_of_e >= icfl_list[icfl_list_size-1]){

                add_in_bit_vector(result->bit_vec,true);
                i++;
                add_in_int_vector(result->distance_from_father,until_next_common_element);
                until_next_common_element=0;
            }

            //2)

            else if(element_of_g >= icfl_list[icfl_list_size-1]){
                
                if(strcmp(S+element_of_g,S+element_of_e)<0){
                    add_in_bit_vector(result->bit_vec,false);
                    j++;
                    until_next_common_element++;
                }

                else{
                    add_in_bit_vector(result->bit_vec,true);
                    i++;
                    add_in_int_vector(result->distance_from_father,until_next_common_element);
                    until_next_common_element=0;
                }
            }

            //3)

            else{

                if(element_of_e > element_of_g){
                    add_in_bit_vector(result->bit_vec,false);
                    j++;
                    until_next_common_element++;
                }

                else{

                    if(strcmp(S+element_of_g,S+element_of_e)<0){
                        add_in_bit_vector(result->bit_vec,false);
                        j++;
                        until_next_common_element++;
                    }

                    else{
                        add_in_bit_vector(result->bit_vec,true);
                        i++;
                        add_in_int_vector(result->distance_from_father,until_next_common_element);
                        until_next_common_element=0;
                    }

                }

            }
        }

    }

    while(j<child->used){
        add_in_bit_vector(result->bit_vec,false);
        j++;
        until_next_common_element++;
    }

    while(i<father->used){
        add_in_bit_vector(result->bit_vec,true);
        i++;
        add_in_int_vector(result->distance_from_father,until_next_common_element);
        until_next_common_element=0;
    }

    return result;
}