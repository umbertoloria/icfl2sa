#include "merge.h"

unsigned long int tot_n_confronti=0,tot_n_confronti_evitati=0,tot_n_confronti_regola=0,tot_n_confronti_strcmp=0;
unsigned long int tot_two_custom=0,tot_one_custom=0;

int n_suffissi_clacolati_sinistra=0,n_suffissi_clacolati_destra=0;

int custom_strcmp_skip=0;

//0 -> x<y
//1 -> y<x

int custom_strcmp_is_less_then(const char *S,int x, int y,std::vector<std::unordered_set<int>>& is_less_then) {
    while (*(S+x) && (*(S+x) == *(S+y))) {
        if(is_less_then[x].find(y) != is_less_then[x].end()) {custom_strcmp_skip++;return -1;}
        if(is_less_then[y].find(x) != is_less_then[y].end()) {custom_strcmp_skip++;return 1;}
        x++;
        y++;
    }
    return *(unsigned char *)(S+x) - *(unsigned char *)(S+y);
}

int custom_strcmp_2_is_less_then(const char *S,int x, int y,std::vector<int>& is_less_then) {
    while (*(S+x) && (*(S+x) == *(S+y))) {
        if(is_less_then[x]==y) {custom_strcmp_skip++;return -1;}
        if(is_less_then[y]==x) {custom_strcmp_skip++;return 1;}
        x++;
        y++;
    }
    return *(unsigned char *)(S+x) - *(unsigned char *)(S+y);
}

//regole standard
int rules(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    
    if(is_custom_suffix[x] && is_custom_suffix[y] ){
        tot_two_custom++;
        tot_n_confronti_strcmp++;
        if(strcmp(S+y+child_offset,S+x+child_offset)<0) return 1;
        else return 0;
    }
    else if(is_custom_suffix[x]){
        tot_one_custom++;
        if(factor_list[x]<=factor_list[y]){
            tot_n_confronti_regola++;
            if(y >= icfl_list[icfl_list_size-1])return 1;
            else return 0;
        } 
        else{
            tot_n_confronti_strcmp++;
            if(strcmp(S+y+child_offset,S+x+child_offset)<0) return 1;
            else return 0;
        }
    }
    else if(is_custom_suffix[y]){
        tot_one_custom++;
        if(factor_list[y]<=factor_list[x]){
            tot_n_confronti_regola++;
            if(x >= icfl_list[icfl_list_size-1]) return 0;
            else return 1;
        }
        else{
            tot_n_confronti_strcmp++;
            if(strcmp(S+y+child_offset,S+x+child_offset)<0) return 1;
            else return 0;
        }
    }
    else if(x >= icfl_list[icfl_list_size-1] && y >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
    else if(factor_list[x]==factor_list[y]){tot_n_confronti_regola++;return 1;}
    else{
        if(x >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
        else if(y >= icfl_list[icfl_list_size-1]){
            tot_n_confronti_strcmp++;
            if(strcmp(S+y+child_offset,S+x+child_offset)<0)return 1;
            else return 0;
        }
        else{
            if(x > y){tot_n_confronti_regola++;return 1;}
            else{
                tot_n_confronti_strcmp++;
                if(strcmp(S+y+child_offset,S+x+child_offset)<0)return 1;
                else return 0;
            }
        }
    }
}

//uso della is less than
int rules_2(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<std::unordered_set<int>>& is_less_then){
    
    if(is_custom_suffix[x] && is_custom_suffix[y] ){
        tot_two_custom++;
        tot_n_confronti_strcmp++;
        if(custom_strcmp_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
        else return 0;
    }
    else if(is_custom_suffix[x]){
        tot_one_custom++;
        if(factor_list[x]<=factor_list[y]){
            tot_n_confronti_regola++;
            if(y >= icfl_list[icfl_list_size-1])return 1;
            else return 0;
        } 
        else{
            tot_n_confronti_strcmp++;
            if(custom_strcmp_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
            else return 0;
        }
    }
    else if(is_custom_suffix[y]){
        tot_one_custom++;
        if(factor_list[y]<=factor_list[x]){
            tot_n_confronti_regola++;
            if(x >= icfl_list[icfl_list_size-1]) return 0;
            else return 1;
        }
        else{
            tot_n_confronti_strcmp++;
            if(custom_strcmp_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
            else return 0;
        }
    }
    else if(x >= icfl_list[icfl_list_size-1] && y >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
    else if(factor_list[x]==factor_list[y]){tot_n_confronti_regola++;return 1;}
    else{
        if(x >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
        else if(y >= icfl_list[icfl_list_size-1]){
            tot_n_confronti_strcmp++;
            if(custom_strcmp_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0)return 1;
            else return 0;
        }
        else{
            if(x > y){tot_n_confronti_regola++;return 1;}
            else{
                tot_n_confronti_strcmp++;
                if(custom_strcmp_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0)return 1;
                else return 0;
            }
        }
    }
}

//uso della strcmp custom e is less than
int rules_3(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, int x, int y, int child_offset,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<int>& is_less_then){
    
    if(is_custom_suffix[x] && is_custom_suffix[y] ){
        tot_two_custom++;
        tot_n_confronti_strcmp++;
        if(custom_strcmp_2_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
        else return 0;
    }
    else if(is_custom_suffix[x]){
        tot_one_custom++;
        if(factor_list[x]<=factor_list[y]){
            tot_n_confronti_regola++;
            if(y >= icfl_list[icfl_list_size-1])return 1;
            else return 0;
        } 
        else{
            tot_n_confronti_strcmp++;
            if(custom_strcmp_2_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
            else return 0;
        }
    }
    else if(is_custom_suffix[y]){
        tot_one_custom++;
        if(factor_list[y]<=factor_list[x]){
            tot_n_confronti_regola++;
            if(x >= icfl_list[icfl_list_size-1]) return 0;
            else return 1;
        }
        else{
            tot_n_confronti_strcmp++;
            if(custom_strcmp_2_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0) return 1;
            else return 0;
        }
    }
    else if(x >= icfl_list[icfl_list_size-1] && y >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
    else if(factor_list[x]==factor_list[y]){tot_n_confronti_regola++;return 1;}
    else{
        if(x >= icfl_list[icfl_list_size-1]){tot_n_confronti_regola++;return 0;}
        else if(y >= icfl_list[icfl_list_size-1]){
            tot_n_confronti_strcmp++;
            if(custom_strcmp_2_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0)return 1;
            else return 0;
        }
        else{
            if(x > y){tot_n_confronti_regola++;return 1;}
            else{
                tot_n_confronti_strcmp++;
                if(custom_strcmp_2_is_less_then(S,y+child_offset,x+child_offset,is_less_then)<0)return 1;
                else return 0;
            }
        }
    }
}

int custom_strcmp_3(const char *S,int x, int y,std::vector<int>& is_less_then) {
    while (*(S+x) && (*(S+x) == *(S+y))) {
        if(is_less_then[x]==y) {custom_strcmp_skip++;return -1;}
        if(is_less_then[y]==x) {custom_strcmp_skip++;return 1;}
        x++;
        y++;
    }
    return *(unsigned char *)(S+x) - *(unsigned char *)(S+y);
}


int pstrcmp2( const void* a, const void* b ){return strcmp( *(const char**)a, *(const char**)b );}

int binarySearch_for_prefix(const char* S, std::vector<int>& father,int child) {
    int low=0,high=father.size()-1;
    if(high==-1) return -1;
    int mid,temp_res;
    while(high>=low){
        mid=(low+high)/2;
        temp_res = std::strcmp(S+child,S+father[mid]);

        if(high == low){
            if (temp_res>0) return mid;
            else return mid-1;
        }

        if (temp_res < 0) high=mid;
        else low=mid+1;
    }
    return -1;
}

int binarySearch_for_prefix_2(const char* S, std::vector<int>& father,int child,int low,int high) {
    if(high==-1) return -1;
    int mid,temp_res;
    while(high>=low){
        mid=(low+high)/2;
        temp_res = std::strcmp(S+child,S+father[mid]);

        if(high == low){
            if (temp_res>0) return mid;
            else return mid-1;
        }

        if (temp_res < 0) high=mid;
        else low=mid+1;
    }
    return -1;
}

int binarySearch_for_prefix_3(const char* S, std::vector<int>& father,int child,int low,int high,int child_offset) {
    if(high==-1) return -1;
    int mid,temp_res;
    while(high>=low){
        mid=(low+high)/2;
        temp_res = strcmp(S+child+child_offset,S+father[mid]+child_offset);

        if(high == low){
            if (temp_res>0) return mid;
            else return mid-1;
        }

        if (temp_res < 0) high=mid;
        else low=mid+1;
    }
    return -1;
}

int binarySearch_for_prefix_4(const char* S, std::vector<int>& father,int child,int low,int high,int child_offset,std::vector<int>& icfl_list, const int& icfl_list_size,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list) {
    if(high==-1) return -1;
    int mid,temp_res;
    while(high>=low){
        mid=(low+high)/2;
        temp_res = rules(S,icfl_list,icfl_list_size,father[mid],child,child_offset,is_custom_suffix,factor_list);

        if(high == low){
            if (temp_res == 0) return mid;
            else return mid-1;
        }

        if (temp_res == 1) high=mid;
        else low=mid+1;
    }
    return -1;
}

int get_factor(vector<int>& icfl_list,int index){
    int max_factor=icfl_list.size()-1;

    for(int i=0;i<max_factor;i++){
        if(index >= icfl_list.at(i) && index < icfl_list.at(i+1)){
            return i;
        }
    }

    return max_factor;

}

std::vector<int> get_factor_list(vector<int>& icfl_list,int str_len){
    std::vector<int> res;
    res.resize(str_len);
    #pragma omp parallel for
    for(int i=0;i<str_len;++i) res.at(i)=get_factor(icfl_list,i);
    return res;
}


std::vector<bool> in_prefix_merge_bit_vector_5(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int> father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix){
    std::vector<bool> result;
    result.resize(father.size()+child.size());
    int temp_res;

    int i=0,j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    //cout<<"Comparing:\n";
    //printVec(father);
    //cout<<"\n";
    //printVec(child);
    //cout<<"\n";

    while( i<father.size() && j<child.size()){

        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0){result.push_back(false);j++;}
            else{result.push_back(true);i++;}
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){result.push_back(true);i++;}
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){result.push_back(false);j++;}
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){result.push_back(true);i++;}
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){result.push_back(false);j++;}
                else{result.push_back(true);i++;}
            }
            else{
                if(father[i] > child[j]){result.emplace_back(false);j++;}
                else{
                    if(strcmp(S+child[j],S+father[i])<0){result.push_back(false);j++;}
                    else{result.push_back(true);i++;}
                }
            }
        }

    }

    while(j<child.size()){result.push_back(false);j++;}
    while(i<father.size()){result.push_back(true);i++;}

    return result;
}

//Migliorata la ricerca del fattore con vettore d'appoggio
std::vector<bool> in_prefix_merge_bit_vector_5_1(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int> father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix,vector<int>& factor_list){
    std::vector<bool> result;
    result.resize(father.size()+child.size());
    int i=0,j=0,temp_res;

    //cout<<"capacity: "<<result.capacity()<<"\n";
    //cout<<j<<"\n";
    //    cout<<i<<"\n";
    //    cout<<i+j<<"\n";

    while( i<father.size() && j<child.size()){
        //cout<<j<<"\n";
        //cout<<i<<"\n";
        //cout<<i+j<<"\n";
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0){result.at(i+j)=false;j++;}
            else{result.at(i+j)=true;i++;}
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){result.at(i+j)=true;i++;}
        else if(factor_list[father[i]]==factor_list[child[j]]){result.at(i+j)=false;j++;}
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){result.at(i+j)=true;i++;}
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){result.at(i+j)=false;j++;}
                else{result.at(i+j)=true;i++;}
            }
            else{
                if(father[i] > child[j]){result.at(i+j)=false;j++;}
                else{
                    if(strcmp(S+child[j],S+father[i])<0){result.at(i+j)=false;j++;}
                    else{result.at(i+j)=true;i++;}
                }
            }
        }

    }
    //cout<<"a\n";
    while(j<child.size()){result.at(i+j)=false;j++;}
    //cout<<"a\n";
    while(i<father.size()){result.at(i+j)=true;i++;}

    return result;
}

//Restituisce direttamente la catena
std::vector<int> in_prefix_merge_bit_vector_5_2(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, vector<int>& child,int father_lenght,std::vector<int>& is_custom_suffix,vector<int>& factor_list){
    std::vector<int> result;
    result.resize(father.size()+child.size());
    int i=0,j=0,temp_res;

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0){result.at(i+j)=child[j];++j;}
            else{result.at(i+j)=father[i];++i;}
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){result.at(i+j)=father[i];++i;}
        else if(factor_list[father[i]]==factor_list[child[j]]){result.at(i+j)=child[j];++j;}
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){result.at(i+j)=father[i];++i;}
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){result.at(i+j)=child[j];++j;}
                else{result.at(i+j)=father[i];++i;}
            }
            else{
                if(father[i] > child[j]){result.at(i+j)=child[j];++j;}
                else{
                    if(strcmp(S+child[j],S+father[i])<0){result.at(i+j)=child[j];++j;}
                    else{result.at(i+j)=father[i];++i;}
                }
            }
        }

    }
    while(j<child.size()){result.at(i+j)=child[j];++j;}
    while(i<father.size()){result.at(i+j)=father[i];++i;}

    return result;
}

std::vector<int> in_prefix_merge_bit_vector_5_3(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int> result;
    result.reserve(father.size()+child.size());
    //cerchiamo la posizione migliore per il primo elemento dei figli
    int best_fit,i=0,j=0;
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    //if(best_fit>=0) result.insert(result.end(),father.begin(),father.begin()+best_fit);
    for(int z=0;z<=best_fit;z++) result.push_back(father.at(z));
    result.push_back(child.at(0));
    j++;
    i=best_fit+1;
    
    //cout<<"best_fit: "<<best_fit<<"\n";

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            if(strcmp(S+child[j],S+father[i])<0) result.push_back(child[j++]);
            else result.push_back(father[i++]);
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    result.insert(result.end(),child.begin()+j,child.end());
    result.insert(result.end(),father.begin()+i,father.end());

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n\n";

    return result;
}


void in_prefix_merge_bit_vector_5_4(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    result.reserve(father.size()+child.size());
    //cout<<"father.size(): "<<father.size()<<"\n";
    //cout<<"child.size(): "<<child.size()<<"\n";
    //cout<<"tot reserved: "<<child.size()+father.size()<<"\n\n";

    int i=0,j=0;

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            if(strcmp(S+child[j],S+father[i])<0) result.push_back(child[j++]);
            else result.push_back(father[i++]);
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    //while(j<child.size())result.push_back(child[j++]);
    result.insert(result.end(),child.begin()+j,child.end());
    //while(i<father.size())result.push_back(father[i++]);
    result.insert(result.end(),father.begin()+i,father.end());
}

void in_prefix_merge_bit_vector_5_5(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    result.reserve(father.size()+child.size());

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int best_fit,i=0,j=0;
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    //if(best_fit>=0) result.insert(result.end(),father.begin(),father.begin()+best_fit);
    for(int z=0;z<=best_fit;z++) result.push_back(father.at(z));
    result.push_back(child.at(0));
    j++;
    i=best_fit+1;
    
    //cout<<"best_fit: "<<best_fit<<"\n";

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            if(strcmp(S+child[j],S+father[i])<0) result.push_back(child[j++]);
            else result.push_back(father[i++]);
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    result.insert(result.end(),child.begin()+j,child.end());
    result.insert(result.end(),father.begin()+i,father.end());

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n\n";
}

//sperimentale, largo uso della ricerca binaria
void in_prefix_merge_bit_vector_5_6(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    result.reserve(father.size()+child.size());

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int best_fit,i=0,j=0,starting_position;
    best_fit=binarySearch_for_prefix(S,father,child.at(j));
    //if(best_fit>=0) result.insert(result.end(),father.begin(),father.begin()+best_fit);
    for(int z=0;z<=best_fit;z++) result.push_back(father.at(z));
    result.push_back(child.at(j));
    j++;
    starting_position=i=best_fit+1;
    
    //cout<<"best_fit: "<<best_fit<<"\n";

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            best_fit=binarySearch_for_prefix_2(S,father,child.at(j),starting_position,father.size()-1);
            for(int z=i;z<=best_fit;z++) result.push_back(father.at(z));
            result.push_back(child.at(j));
            j++;
            i=best_fit+1;
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    result.insert(result.end(),child.begin()+j,child.end());
    result.insert(result.end(),father.begin()+i,father.end());

}

void in_prefix_merge_bit_vector_5_7(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    result.reserve(father.size()+child.size());

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    std::vector<int> temp_father,temp_res;
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    //parto da un punto ragionevole
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    starting_position=best_fit+1;
    //finché non ne trovo uno maggiore o uguale
    for(int i=starting_position;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father==-1){min_father=father.size()-1;max_father=father.size();}
    if(strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<"\n";
        if(min_father>0) result.insert(result.end(),father.begin(),father.begin()+min_father);
        result.insert(result.end(),child.begin(),child.end());
        result.insert(result.end(),father.begin()+min_father,father.end());
        //cout<<"result: \n";
        //printVec(result);
        //cout<<"\n";
        return;
    }
    else if(min_father==father.size()-1 && strncmp(S+child.at(0),S+father.at(min_father),child_offset)>0){
        //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<"\n";
        result.insert(result.end(),father.begin(),father.end());
        result.insert(result.end(),child.begin(),child.end());
        //cout<<"result: \n";
        //printVec(result);
        //cout<<"\n";
        return;
    }
    else{
        //finché non ne trovo uno diverso
        for(int i=min_father+1;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)!=0){max_father=i;break;}
        if(max_father==-1) max_father=father.size();
    }
    //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<"\n";

    //costruisco il padre temporaeo
    temp_father.insert(temp_father.end(),father.begin()+min_father,father.begin()+max_father);
    //cout<<"temp_father: \n";
    //printVec(temp_father);
    //cout<<"temp_father_size:"<<temp_father.size()<<"\n";
    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=0,j=0,father_offset=father_node->suffix_len;
    temp_res.reserve(father.size()+child.size());
    //operiamo sui risultati temporanei (padre e risultati, i figli rimangono uguali)
    while( i<temp_father.size() && j<child.size()){
        if(is_custom_suffix[temp_father[i]] && is_custom_suffix[child[j]] ){
            best_fit=binarySearch_for_prefix_3(S,temp_father,child.at(j),i,temp_father.size()-1,child_offset);
            //best_fit=binarySearch_for_prefix_2(S,temp_father,child.at(j),starting_position,temp_father.size()-1);
            //for(int z=i;z<=best_fit;z++) temp_res.push_back(temp_father.at(z));
            temp_res.insert(temp_res.end(),temp_father.begin()+i,temp_father.begin()+best_fit+1);
            temp_res.push_back(child.at(j));
            j++;
            i=best_fit+1;
            //usare questo significherebbe che l'utilizzo dei vettori temporaneri è inutile
            //if(strcmp(S+child[j]+child_offset,S+temp_father[i]+child_offset)<0) temp_res.push_back(child[j++]);
            //else temp_res.push_back(temp_father[i++]);
        }
        else if(is_custom_suffix[temp_father[i]]){
            if(factor_list[temp_father[i]]<=factor_list[child[j]]){
                if(child[j] >= icfl_list[icfl_list_size-1])temp_res.push_back(child[j++]);
                else temp_res.push_back(temp_father[i++]);
            } 
            else{
                best_fit=binarySearch_for_prefix_3(S,temp_father,child.at(j),i,temp_father.size()-1,child_offset);
                temp_res.insert(temp_res.end(),temp_father.begin()+i,temp_father.begin()+best_fit+1);
                temp_res.push_back(child.at(j));
                j++;
                i=best_fit+1;
                //if(strcmp(S+child[j]+child_offset,S+temp_father[i]+child_offset)<0) temp_res.push_back(child[j++]);
                //else temp_res.push_back(temp_father[i++]);
            }
        }

        else if(is_custom_suffix[child[j]]){
            if(factor_list[child[j]]<=factor_list[temp_father[i]]){
                if(temp_father[i] >= icfl_list[icfl_list_size-1]) temp_res.push_back(temp_father[i++]);
                else temp_res.push_back(child[j++]);
            }
            else{
                best_fit=binarySearch_for_prefix_3(S,temp_father,child.at(j),i,temp_father.size()-1,child_offset);
                temp_res.insert(temp_res.end(),temp_father.begin()+i,temp_father.begin()+best_fit+1);
                temp_res.push_back(child.at(j));
                j++;
                i=best_fit+1;
                //if(strcmp(S+child[j]+child_offset,S+temp_father[i]+child_offset)<0) temp_res.push_back(child[j++]);
                //else temp_res.push_back(temp_father[i++]);
            }
        }

        else if(temp_father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])temp_res.push_back(temp_father[i++]);
        else if(factor_list[temp_father[i]]==factor_list[child[j]])temp_res.push_back(child[j++]);
        else{
            if(temp_father[i] >= icfl_list[icfl_list_size-1])temp_res.push_back(temp_father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j]+child_offset,S+temp_father[i]+child_offset)<0)temp_res.push_back(child[j++]);
                else temp_res.push_back(temp_father[i++]);
            }
            else{
                if(temp_father[i] > child[j])temp_res.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j]+child_offset,S+temp_father[i]+child_offset)<0)temp_res.push_back(child[j++]);
                    else temp_res.push_back(temp_father[i++]);
                }
            }
        }

    }
    //cout<<"i: "<<i<<", j: "<<j<<"\n";
    temp_res.insert(temp_res.end(),child.begin()+j,child.end());
    temp_res.insert(temp_res.end(),temp_father.begin()+i,temp_father.end());

    //ora che abbiamo i risultati temporanei, uniamo tutto
    if(min_father>0) result.insert(result.end(),father.begin(),father.begin()+min_father);
    result.insert(result.end(),temp_res.begin(),temp_res.end());
    if(max_father<father.size()) result.insert(result.end(),father.begin()+max_father,father.end());

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

//max_father calcilato con ricerca binaria, eliminazione array di appoggio
void in_prefix_merge_bit_vector_5_8(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    result.reserve(father.size()+child.size());

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    //parto da un punto ragionevole
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    starting_position=best_fit+1;
    //finché non ne trovo uno maggiore o uguale
    for(int i=starting_position;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father==-1){min_father=father.size()-1;max_father=father.size();}
    if(strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<"\n";
        if(min_father>0) result.insert(result.end(),father.begin(),father.begin()+min_father);
        result.insert(result.end(),child.begin(),child.end());
        result.insert(result.end(),father.begin()+min_father,father.end());
        //cout<<"result: \n";
        //printVec(result);
        //cout<<"\n";
        return;
    }
    else if(min_father==father.size()-1 && strncmp(S+child.at(0),S+father.at(min_father),child_offset)>0){
        //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<"\n";
        result.insert(result.end(),father.begin(),father.end());
        result.insert(result.end(),child.begin(),child.end());
        //cout<<"result: \n";
        //printVec(result);
        //cout<<"\n";
        return;
    }
    else{
        //finché non ne trovo uno diverso
        //for(int i=min_father+1;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)!=0){max_father=i;break;}
        //if(max_father==-1) max_father=father.size();
        max_father=binarySearch_for_prefix(S,father,child.at(child.size()-1))+1;
    }
    //cout<<"result size: "<<father.size()+child.size()<<"\n";
    //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<", difference: "<<max_father-min_father<<"\n";

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=min_father,j=0,father_offset=father_node->suffix_len;
    //operiamo sui risultati temporanei (padre e risultati, i figli rimangono uguali)
    result.insert(result.end(),father.begin(),father.begin()+min_father);
    //cout<<"result solo padre: \n";
    //printVec(result);
    while( i<max_father && j<child.size()){
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]] ){
            best_fit=binarySearch_for_prefix_3(S,father,child.at(j),i,max_father-1,child_offset);
            result.insert(result.end(),father.begin()+i,father.begin()+best_fit+1);
            result.push_back(child.at(j));
            j++;
            i=best_fit+1;
        }
        else if(is_custom_suffix[father[i]]){
            if(factor_list[father[i]]<=factor_list[child[j]]){
                if(child[j] >= icfl_list[icfl_list_size-1])result.push_back(child[j++]);
                else result.push_back(father[i++]);
            } 
            else{
                best_fit=binarySearch_for_prefix_3(S,father,child.at(j),i,max_father-1,child_offset);
                result.insert(result.end(),father.begin()+i,father.begin()+best_fit+1);
                result.push_back(child.at(j));
                j++;
                i=best_fit+1;
            }
        }

        else if(is_custom_suffix[child[j]]){
            if(factor_list[child[j]]<=factor_list[father[i]]){
                if(father[i] >= icfl_list[icfl_list_size-1]) result.push_back(father[i++]);
                else result.push_back(child[j++]);
            }
            else{
                best_fit=binarySearch_for_prefix_3(S,father,child.at(j),i,max_father-1,child_offset);
                result.insert(result.end(),father.begin()+i,father.begin()+best_fit+1);
                result.push_back(child.at(j));
                j++;
                i=best_fit+1;
            }
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j]+child_offset,S+father[i]+child_offset)<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j]+child_offset,S+father[i]+child_offset)<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    //cout<<"i: "<<i<<", j: "<<j<<"\n";
    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<father.size()) result.insert(result.end(),father.begin()+i,father.end());

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

//salva nel risultato solo gli indici dove cambia l'array degli indici del padre e l'array,gang
void in_prefix_merge_bit_vector_5_9(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    //parto da un punto ragionevole
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    starting_position=best_fit+1;
    //finché non ne trovo uno maggiore o uguale
    //for(int i=starting_position;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    //cout<<"starting_position: "<<starting_position<<", father.size(): "<<father.size()<<"\n";

    //valuto se deve essere inserito in testa a tutto o in coda a tutto
    if(starting_position==father.size()){
        if(strncmp(S+child.at(0),S+father.at(father.size()-1),child_offset)>0){
            //da qui si evince che se i figli vanno messi in coda, allora min_father sarà uguale a -1
            min_father=-1;
            result=child;
            return;
        }
        else starting_position--;
    }

    if(strncmp(S+child.at(0),S+father.at(0),child_offset)==0) min_father=0;
    else
        for(int i=starting_position;i>=0;i--){
            if(strncmp(S+child.at(0),S+father.at(i),child_offset)>0){min_father=i+1;break;}
        }

    //non ci sono stringhe con child_offset caratteri uguali quindi si parte dal primo più grande
    if(min_father==-1) min_father=starting_position;

    //cout<<"new min_father: "<<min_father<<"\n";
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    
    
    
    ////i figli non vanno messi in coda ma da min_father in poi
    int finish_position;
    //da questo punto in poi c'è almeno una stringa in father che ha almeno child_offset caratteri uguali
    finish_position=binarySearch_for_prefix(S,father,child.at(child.size()-1))+1;
    //finché non ne trovo uno diverso
    for(int i=finish_position;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)!=0){ max_father=i;break;}
    if(max_father==-1) max_father=father.size();
    //max_father=binarySearch_for_prefix(S,father,child.at(child.size()-1))+1;
    
    //cout<<"result size: "<<father.size()+child.size()<<"\n";
    //cout<<"min_father: "<<min_father<<", max_father: "<<max_father<<", difference: "<<max_father-min_father<<"\n";

    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=min_father,j=0,father_offset=father_node->suffix_len;
    
    result.reserve((max_father-min_father)+1+child.size());

    while( i<max_father && j<child.size()){
        if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0) result.push_back(father[i++]);
        else result.push_back(child[j++]);
    }
    
    //cout<<"i: "<<i<<", j: "<<j<<"\n";
    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<father.size()) result.insert(result.end(),father.begin()+i,father.begin()+max_father);

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

void in_prefix_merge_bit_vector_5_9_2(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<suffix_tree_node*>& indice_nodo,std::unordered_map<suffix_tree_node*,int>& ordered_nodes_list){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    best_fit=binarySearch_for_prefix(S,father,child.at(0));
    starting_position=best_fit+1;
    if(starting_position==father.size()){
        if(strncmp(S+child.at(0),S+father.at(father.size()-1),child_offset)>0){
            min_father=-1;
            result=child;
            return;
        }
        else starting_position--;
    }

    if(strncmp(S+child.at(0),S+father.at(0),child_offset)==0) min_father=0;
    else
        for(int i=starting_position;i>=0;i--){
            if(strncmp(S+child.at(0),S+father.at(i),child_offset)>0){min_father=i+1;break;}
        }
    if(min_father==-1) min_father=starting_position;

    child_node->min_father=min_father;
    child_node->max_father=max_father;

    int finish_position;
    finish_position=binarySearch_for_prefix(S,father,child.at(child.size()-1))+1;
    for(int i=finish_position;i<father.size();i++) if(strncmp(S+child.at(0),S+father.at(i),child_offset)!=0){ max_father=i;break;}
    if(max_father==-1) max_father=father.size();
    
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    int i=min_father,j=0,father_offset=father_node->suffix_len;
    
    result.reserve((max_father-min_father)+1+child.size());
    int temp_father,temp_child;
    while( i<max_father && j<child.size()){
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            temp_father=father[i];
            temp_child=child[j];
            while (1)
            {
                /* code */
            }
            
        }
        if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0) result.push_back(father[i++]);
        else result.push_back(child[j++]);
    }
    
    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<father.size()) result.insert(result.end(),father.begin()+i,father.begin()+max_father);
}

//salva nel risultato solo gli indici dove cambia l'array degli indici del padre e l'array,gang
//riceca lineare degli indici
void in_prefix_merge_bit_vector_5_10(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int>& father=father_node->common_chain_of_suffiexes,&child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=0;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father == -1 || strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        child_node->min_father=min_father;
        result=child;
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=min_father,j=0,father_offset=father_node->suffix_len;
    
    result.reserve((max_father-min_father)+1+child.size());
    //cout<<"number of indexes in chain: "<<(max_father-min_father)+1+child.size()<<"\n";
    //cout<<"number of indexes in child: "<<child.size()<<"\n";

    while( i<max_father && j<child.size()){
        if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0) result.push_back(father[i++]);
        else result.push_back(child[j++]);
    }

    //cout<<"number of iterations: "<<num_of_iterations<<"\n";

    //cout<<"i: "<<i<<", j: "<<j<<"\n";
    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<father.size()) result.insert(result.end(),father.begin()+i,father.begin()+max_father);

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

//uas il merge, non funziona
void in_prefix_merge_bit_vector_5_10_2(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=0;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father == -1 || strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        child_node->min_father=min_father;
        result=child;
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=min_father,j=0,father_offset=father_node->suffix_len;
    
    result.reserve((max_father-min_father)+1+child.size());

    std::merge(
           father.begin()+i,
           father.begin()+max_father,
           child.begin(),
           child.end(),
           std::back_inserter(result),
           [S,&icfl_list,icfl_list_size,child_offset,&is_custom_suffix,&factor_list](int a, int b) {
               //int temp_res=custom_strcmp(S,icfl_list,icfl_list_size,a,b,child_offset,is_custom_suffix,factor_list,indice);
               //if(temp_res<0) return 1;
               return 0;
           });

}

//salva nel risultato solo gli indici dove cambia l'array degli indici del padre e l'array,gang
//riceca lineare degli indici
//usa il sorting
void in_prefix_merge_bit_vector_5_11(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    std::vector<int> father=father_node->common_chain_of_suffiexes,child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}

    std::vector<int> unknown_indexes;
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=0;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father == -1 || strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        child_node->min_father=min_father;
        result=child;
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    //cerchiamo la posizione migliore per il primo elemento dei figli
    int i=min_father,j=0,father_offset=father_node->suffix_len;
    
    result.reserve((max_father-min_father)+1+child.size());

    while( i<max_father && j<child.size()){
        if(is_custom_suffix[father[i]] || is_custom_suffix[child[j]] ){
            unknown_indexes.insert(unknown_indexes.end(),father.begin()+i,father.begin()+max_father);
            unknown_indexes.insert(unknown_indexes.end(),child.begin()+j,child.begin()+child.size());
            quicksort_of_indexes_5_2(S,unknown_indexes,child_offset);
            result.insert(result.end(),unknown_indexes.begin(),unknown_indexes.end());
            unknown_indexes.clear();
            i=max_father;
            j=child.size();
        }

        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
        else if(factor_list[father[i]]==factor_list[child[j]])result.push_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1])result.push_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j]+child_offset,S+father[i]+child_offset)<0)result.push_back(child[j++]);
                else result.push_back(father[i++]);
            }
            else{
                if(father[i] > child[j])result.push_back(child[j++]);
                else{
                    if(strcmp(S+child[j]+child_offset,S+father[i]+child_offset)<0)result.push_back(child[j++]);
                    else result.push_back(father[i++]);
                }
            }
        }

    }
    //cout<<"i: "<<i<<", j: "<<j<<"\n";
    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<father.size()) result.insert(result.end(),father.begin()+i,father.begin()+max_father);

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

std::vector<int> in_prefix_merge_bit_vector_6(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child){
    std::vector<int> result;
    result.reserve(father.size()+child.size());

    int i=0,j=0;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && j<child.size()){
        if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]){
            result.emplace_back(father[i]);
            i++;
        }
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])){
            result.emplace_back(child[j]);
            j++;
        }
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]){
                result.emplace_back(father[i]);
                i++;
            }
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    result.emplace_back(father[i]);
                    i++;
                }
            }
            else{
                if(father[i] > child[j]){
                    result.emplace_back(child[j]);
                    j++;
                }
                else{
                    if(strcmp(S+child[j],S+father[i])<0){
                        result.emplace_back(child[j]);
                        j++;
                    }
                    else{
                        result.emplace_back(father[i]);
                        i++;
                    }
                }

            }
        }

    }

    while(j<child.size()){
        result.emplace_back(child[j]);
        j++;
    }

    while(i<father.size()){
        result.emplace_back(father[i]);
        i++;
    }

    return result;
}

std::vector<int> in_prefix_merge_bit_vector_7(const char* S, vector<int> icfl_list, int icfl_list_size, vector<int> father, vector<int> child,vector<int> is_custom_suffix){
    std::vector<int> result;
    result.reserve(father.size()+child.size());

    int i=0,j=0,temp_res;

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && j<child.size()){
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child[j],S+father[i]);
            if(temp_res<0) result.emplace_back(child[j++]);
            else result.emplace_back(father[i++]);
        }
        //A questo punto solo uno o nessuno dei due è arificiale
        //Se il primo è artificiale allora si inverte la regola
        else if (is_custom_suffix[father[i]]){
            if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) result.emplace_back(child[j++]);
            else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])) result.emplace_back(father[i++]);
            else{
                if(father[i] >= icfl_list[icfl_list_size-1]) result.emplace_back(child[j++]);
                else if(child[j] >= icfl_list[icfl_list_size-1]){
                    if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(father[i++]);
                    else result.emplace_back(child[j++]);
                }
                else{
                    if(father[i] > child[j]) result.emplace_back(father[i++]);
                    else{
                        //Questo uguale all'originale, c'è la strcmp 
                        if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                        else result.emplace_back(father[i++]);
                    }

                }
            }
        }
        //Se nessuno dei due è custom:
        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) result.emplace_back(father[i++]);
        else if(get_factor(icfl_list,father[i])==get_factor(icfl_list,child[j])) result.emplace_back(child[j++]);
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]) result.emplace_back(father[i++]);
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                else result.emplace_back(father[i++]);
            }
            else{
                if(father[i] > child[j]) result.emplace_back(child[j++]);
                else{
                    if(strcmp(S+child[j],S+father[i])<0) result.emplace_back(child[j++]);
                    else result.emplace_back(father[i++]);
                }

            }
        }

    }

    while(j<child.size()) result.emplace_back(child[j++]);

    while(i<father.size()) result.emplace_back(father[i++]);

    return result;
}

void in_prefix_merge_bit_vector_8(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, int child,vector<int>& is_custom_suffix,int father_lenght,vector<int>& factor_list){

    int i=0,j=0,temp_res;
    bool flag=father[i];

    //cout<<" last:"<<icfl_list[icfl_list.size()-1]<<" ";

    while( i<father.size() && flag){
        if(is_custom_suffix[father[i]] && is_custom_suffix[child]){
            //cout<<i<<" "<<j<<"\n";
            temp_res = strcmp(S+child+father_lenght,S+father[i]+father_lenght);
            if(temp_res<0) flag=false;
            else ++i;
        }
        //A questo punto solo uno o nessuno dei due è arificiale
        //Se il primo è artificiale allora si inverte la regola
        else if (is_custom_suffix[father[i]]){
            if(father[i] >= icfl_list[icfl_list_size-1] && child >= icfl_list[icfl_list_size-1]) flag=false;
            else if(factor_list[father[i]]==factor_list[child]) ++i;
            else{
                if(father[i] >= icfl_list[icfl_list_size-1]) flag=false;
                else if(child >= icfl_list[icfl_list_size-1]){
                    if(strcmp(S+child+father_lenght,S+father[i]+father_lenght)<0) ++i;
                    else flag=false;
                }
                else{
                    if(father[i] > child) ++i;
                    else{
                        //Questo uguale all'originale, c'è la strcmp 
                        if(strcmp(S+child+father_lenght,S+father[i]+father_lenght)<0) flag=false;
                        else ++i;
                    }

                }
            }
        }
        //Se nessuno dei due è custom:
        else if(father[i] >= icfl_list[icfl_list_size-1] && child >= icfl_list[icfl_list_size-1]) ++i;
        else if(factor_list[father[i]]==factor_list[child]) flag=false;
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]) ++i;
            else if(child >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child+father_lenght,S+father[i]+father_lenght)<0) flag=false;
                else ++i;
            }
            else{
                if(father[i] > child) flag=false;
                else{
                    if(strcmp(S+child+father_lenght,S+father[i]+father_lenght)<0) flag=false;
                    else ++i;
                }

            }
        }

    }

    if(!flag) father.insert(father.begin()+i,child);
    else father.push_back(child);
}


std::vector<int> in_prefix_merge_bit_vector_9(const char* S, vector<int>& icfl_list, int icfl_list_size, vector<int>& father, vector<int>& child,vector<int>& is_custom_suffix,int father_lenght,vector<int>& factor_list){
    std::vector<int> result;
    result.resize(father.size()+child.size());
    int i=0,j=0,temp_res;

    while( i<father.size() && j<child.size()){
        
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            temp_res = strcmp(S+child[j]+father_lenght,S+father[i]+father_lenght);
            if(temp_res<0) {result.at(i+j)=child[j];++j;}
            else {result.at(i+j)=father[i];++i;}
        }
        //A questo punto solo uno o nessuno dei due è arificiale
        //Se il primo è artificiale allora si inverte la regola
        else if (is_custom_suffix[father[i]]){
            if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) {result.at(i+j)=child[j];++j;}
            else if(factor_list[father[i]]==factor_list[child[j]]) {result.at(i+j)=father[i];++i;}
            else{
                if(father[i] >= icfl_list[icfl_list_size-1]) {result.at(i+j)=child[j];++j;}
                else if(child[j] >= icfl_list[icfl_list_size-1]){
                    if(strcmp(S+child[j]+father_lenght,S+father[i]+father_lenght)<0) {result.at(i+j)=father[i];++i;}
                    else {result.at(i+j)=child[j];++j;}
                }
                else{
                    if(father[i] > child[j]) {result.at(i+j)=father[i];++i;}
                    else{
                        //Questo uguale all'originale, c'è la strcmp 
                        if(strcmp(S+child[j]+father_lenght,S+father[i]+father_lenght)<0) {result.at(i+j)=child[j];++j;}
                        else {result.at(i+j)=father[i];++i;};
                    }

                }
            }
        }
        //Se nessuno dei due è custom:
        else if(father[i] >= icfl_list[icfl_list_size-1] && child[j] >= icfl_list[icfl_list_size-1]) {result.at(i+j)=father[i];++i;}
        else if(factor_list[father[i]]==factor_list[child[j]]) {result.at(i+j)=child[j];++j;}
        else{
            if(father[i] >= icfl_list[icfl_list_size-1]) {result.at(i+j)=father[i];++i;}
            else if(child[j] >= icfl_list[icfl_list_size-1]){
                if(strcmp(S+child[j]+father_lenght,S+father[i]+father_lenght)<0) {result.at(i+j)=child[j];++j;}
                else {result.at(i+j)=father[i];++i;};
            }
            else{
                if(father[i] > child[j]) {result.at(i+j)=child[j];++j;}
                else{
                    if(strcmp(S+child[j]+father_lenght,S+father[i]+father_lenght)<0) {result.at(i+j)=child[j];++j;}
                    else {result.at(i+j)=father[i];++i;}
                }

            }
        }

    }
    
    while(j<child.size()) {result.at(i+j)=child[j];++j;}
    while(i<father.size()) {result.at(i+j)=father[i];++i;}

    return result;
}


//non funziona
void in_prefix_merge_bit_vector_10(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<std::unordered_set<int>>& is_less_then){
    
    std::vector<int>& father=father_node->common_chain_of_suffiexes,&child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=0;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father == -1 || strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        child_node->min_father=min_father;
        result=child;
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    int i=min_father,j=0,father_offset=father_node->suffix_len;
    int equal_offset;
    bool check_rules;
    int n_confronti=0,n_confronti_evitati=0;
    
    result.reserve((max_father-min_father)+1+child.size());

    while( i<max_father && j<child.size()){
        n_confronti++;
        check_rules=true;
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            if(is_less_then[father[i]].find(child[j]) != is_less_then[father[i]].end())
                {result.push_back(father[i++]);check_rules=false;}
            else if(is_less_then[child[j]].find(father[i]) != is_less_then[child[j]].end())
                {result.push_back(child[j++]);check_rules=false;}
            if(check_rules==false) n_confronti_evitati++;
        }
        if(check_rules){
            //if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0){
            if(rules_2(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list,is_less_then)==0){
                for (equal_offset=0;father[i]-equal_offset>=0 && child[j]-equal_offset>=0 && *(S+father[i]-equal_offset)==*(S+child[j]-equal_offset) && is_less_then[father[i]-equal_offset].find(child[j]-equal_offset) == is_less_then[father[i]-equal_offset].end();equal_offset++)
                    {is_less_then[father[i]-equal_offset].insert(child[j]-equal_offset);n_suffissi_clacolati_sinistra++;}
                //for (equal_offset=father_node->suffix_len;*(S+father[i]+equal_offset)==*(S+child[j]+equal_offset) && is_less_then[father[i]+equal_offset].find(child[j]+equal_offset) == is_less_then[father[i]+equal_offset].end();equal_offset++)
                //    {is_less_then[father[i]+equal_offset].insert(child[j]+equal_offset);n_suffissi_clacolati_destra++;}

                result.push_back(father[i++]);
            }
            else {
                for (equal_offset=0;father[i]-equal_offset>=0 && child[j]-equal_offset>=0 && *(S+father[i]-equal_offset)==*(S+child[j]-equal_offset) &&  is_less_then[child[j]-equal_offset].find(father[i]-equal_offset) == is_less_then[child[j]-equal_offset].end();equal_offset++)
                    {is_less_then[child[j]-equal_offset].insert(father[i]-equal_offset);n_suffissi_clacolati_sinistra++;}
                //for (equal_offset=father_node->suffix_len;*(S+father[i]+equal_offset)==*(S+child[j]+equal_offset) && is_less_then[child[j]+equal_offset].find(father[i]+equal_offset) == is_less_then[child[j]+equal_offset].end();equal_offset++)
                //    {is_less_then[child[j]+equal_offset].insert(father[i]+equal_offset);n_suffissi_clacolati_destra++;}
                result.push_back(child[j++]);
            }
        }
        
    }
    //cout<<"Numero confronti: "<<n_confronti<<", Numero confronti evitati: "<<n_confronti_evitati<<"\n";
    tot_n_confronti+=n_confronti;
    tot_n_confronti_evitati+=n_confronti_evitati;
    
    if(j<child.size()) {
        result.insert(result.end(),child.begin()+j,child.end());
        //for(;j<child.size();j++){
        //    for (equal_offset=0;father[max_father-1]-equal_offset>=0 && child[j]-equal_offset>=0 && *(S+father[max_father-1]-equal_offset)==*(S+child[j]-equal_offset) && is_less_then[father[max_father-1]-equal_offset].find(child[j]-equal_offset) == is_less_then[father[max_father-1]-equal_offset].end();equal_offset++)
        //        {is_less_then[father[max_father-1]-equal_offset].insert(child[j]-equal_offset);n_suffissi_clacolati_sinistra++;}
        //}
    }
    if(i<max_father) {
        result.insert(result.end(),father.begin()+i,father.begin()+max_father);
        //for(;i<max_father;++i){
        //    for (equal_offset=0;father[i]-equal_offset>=0 && child[child.size()-1]-equal_offset>=0 && *(S+father[i]-equal_offset)==*(S+child[child.size()-1]-equal_offset) &&  is_less_then[child[child.size()-1]-equal_offset].find(father[i]-equal_offset) == is_less_then[child[child.size()-1]-equal_offset].end();equal_offset++)
        //        {is_less_then[child[child.size()-1]-equal_offset].insert(father[i]-equal_offset);n_suffissi_clacolati_sinistra++;}
        //}
    }

    //cout<<"result: \n";
    //printVec(result);
    //cout<<"\n";
}

void in_prefix_merge_bit_vector_10_2(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* father_node, suffix_tree_node* child_node,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<int> & is_less_then){
    
    std::vector<int>& father=father_node->common_chain_of_suffiexes,&child=child_node->array_of_indexes;
    if(father.empty()){result=child;return;}
    

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,starting_position,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=0;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    if(min_father == -1 || strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        child_node->min_father=min_father;
        result=child;
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    int i=min_father,j=0,father_offset=father_node->suffix_len;
    int equal_offset;
    bool check_rules;
    int n_confronti=0,n_confronti_evitati=0;
    
    result.reserve((max_father-min_father)+1+child.size());

    while( i<max_father && j<child.size()){
        n_confronti++;
        check_rules=true;
        if(is_custom_suffix[father[i]] && is_custom_suffix[child[j]]){
            if(is_less_then[father[i]] == child[j])
                {result.push_back(father[i++]);check_rules=false;}
            else if(is_less_then[child[j]] == father[i])
                {result.push_back(child[j++]);check_rules=false;}
            if(check_rules==false) n_confronti_evitati++;
        }
        if(check_rules){
            if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0){
            //if(rules_3(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list,is_less_then)==0){
                for (equal_offset=1;*(S+father[i]-equal_offset)==*(S+child[j]-equal_offset) && father[i]-equal_offset>=0 && child[j]-equal_offset>=0 ;equal_offset++)
                    is_less_then[father[i]-equal_offset]=child[j]-equal_offset;

                result.push_back(father[i++]);
            }
            else {
                for (equal_offset=1;*(S+father[i]-equal_offset)==*(S+child[j]-equal_offset) && father[i]-equal_offset>=0 && child[j]-equal_offset>=0 ;equal_offset++)
                    is_less_then[child[j]-equal_offset]=father[i]-equal_offset;
                result.push_back(child[j++]);
            }
        }
        
    }
    //cout<<"Numero confronti: "<<n_confronti<<", Numero confronti evitati: "<<n_confronti_evitati<<"\n";
    tot_n_confronti+=n_confronti;
    tot_n_confronti_evitati+=n_confronti_evitati;

    if(j<child.size()) result.insert(result.end(),child.begin()+j,child.end());
    if(i<max_father) result.insert(result.end(),father.begin()+i,father.begin()+max_father);
}

//nuova implementazione
//il padre ottimo è il nodo che condivide il primo o i primi caratteri con tutti i suoi figli
void in_prefix_merge_bit_vector_11(const char* S, std::vector<int>& icfl_list, const int& icfl_list_size, suffix_tree_node* child_node,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list,std::vector<int> & is_less_then){
    suffix_tree_node* opt_father=child_node;
    while(opt_father->father->father!=NULL) opt_father=opt_father->father;
    if(opt_father==child_node) return;
    //da questo momento in poi è almeno un figlio dell'opt_node
    std::vector<int>& father=opt_father->array_of_indexes,&child=child_node->array_of_indexes;

    //cout<<"father: \n";
    //printVec(father);
    //cout<<"child: \n";
    //printVec(child);
    int starting_position=0;

    if(child_node->father!=opt_father){
        if(child_node->father->min_father==-1){
            opt_father->index_to_nodes[father.size()].insert(opt_father->index_to_nodes[father.size()].end(),child.begin(),child.end());
            return;
        }
        else if(child_node->father->max_father==-1){
            opt_father->index_to_nodes[child_node->father->min_father].insert(opt_father->index_to_nodes[child_node->father->min_father].end(),child.begin(),child.end());
            return;
        }
        else starting_position=child_node->father->min_father;

    }

    //creo un vettore che di interi che hanno i primi child->suffix_len caratteri uguali al padre
    int best_fit,min_father=-1,max_father=-1,child_offset=child_node->suffix_len;
    
    //finché non ne trovo uno maggiore o uguale
    for(int i=starting_position;i<father.size();++i) if(strncmp(S+child.at(0),S+father.at(i),child_offset)<=0){min_father=i;break;}
    child_node->min_father=min_father;
    if(min_father == -1){
        opt_father->index_to_nodes[father.size()].insert(opt_father->index_to_nodes[father.size()].end(),child.begin(),child.end());
        return;
    }
    if(strncmp(S+child.at(0),S+father.at(min_father),child_offset)<0){
        opt_father->index_to_nodes[min_father].insert(opt_father->index_to_nodes[min_father].end(),child.begin(),child.end());
        return;
    }
    for(int i=min_father;i<father.size() && strncmp(S+child.at(0),S+father.at(i),child_offset)==0;++i) max_father=i+1;
    
    //qui max father potrebbe avere anche avere un valore effettivo
    child_node->min_father=min_father;
    child_node->max_father=max_father;

    int i=min_father,j=0;
    int equal_offset;
    bool check_rules;
    int n_confronti=0,n_confronti_evitati=0;

    while( i<max_father && j<child.size()){
        if(rules(S,icfl_list,icfl_list_size,father[i],child[j],child_offset,is_custom_suffix,factor_list)==0) ++i;
        else opt_father->index_to_nodes[i].push_back(child[j++]);
    }

    opt_father->index_to_nodes[i].insert(opt_father->index_to_nodes[i].end(),child.begin()+j,child.end());
    //cout<<"Numero confronti: "<<n_confronti<<", Numero confronti evitati: "<<n_confronti_evitati<<"\n";
    tot_n_confronti+=n_confronti;
    tot_n_confronti_evitati+=n_confronti_evitati;
}



void alternative_prefix_merge_bit_vector(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    result=father;
    int best_fit,i=0;
    if(result.empty()){ result.push_back(child.at(0));i=1;}
    for(;i<child.size();++i){
        best_fit=binarySearch_for_prefix(S,result,child.at(i));
        result.insert(result.begin()+best_fit+1,child.at(i));
    }
}


void alternative_prefix_merge_bit_vector_2(const char* S, std::vector<int>& icfl_list, int icfl_list_size, std::vector<int>& father, std::vector<int>& child,std::vector<int>& result,std::vector<int>& is_custom_suffix,std::vector<int>& factor_list){
    result=father;
    result.insert(result.end(),child.begin(),child.end());
    int n_elements=result.size();
    const char** strings = (const char**)malloc(sizeof(*strings)*n_elements);
    for(int i=0;i<n_elements;++i) strings[i]=S+result.at(i);
    qsort(strings,n_elements,sizeof(strings[0]),pstrcmp2);
    for(int i=0;i<n_elements;++i) result.at(i)=(std::uintptr_t)strings[i]-(std::uintptr_t)S;
}

void printConfrontiEvitati(){
    cout<<"Numero confronti: "<<tot_n_confronti<<"\n";
    cout<<"Numero confronti evitati: "<<tot_n_confronti_evitati<<"\n";
    cout<<"Numero confronti evitati con le regole: "<<tot_n_confronti_regola<<"\n";
    cout<<"Numero confronti con strcmp: "<<tot_n_confronti_strcmp<<"\n";
    cout<<"Numero confronti tra due custom: "<<tot_two_custom<<"\n";
    cout<<"Numero confronti tra un custom e uno canonico: "<<tot_one_custom<<"\n";
    cout<<"Numero coppie calolate verso sinistra:  "<<n_suffissi_clacolati_sinistra<<"\n";
    cout<<"Numero coppie calolate verso destra:  "<<n_suffissi_clacolati_destra<<"\n";
    cout<<"Numero skip della custom_strcmp:  "<<custom_strcmp_skip<<"\n";
}